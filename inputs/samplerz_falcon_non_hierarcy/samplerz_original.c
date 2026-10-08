#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FPR_NORM64(m, e)                                 \
    do                                                   \
    {                                                    \
        uint32_t nt;                                     \
                                                         \
        (e) -= 63;                                       \
                                                         \
        nt = (uint32_t)((m) >> 32);                      \
        nt = (nt | -nt) >> 31;                           \
        (m) ^= ((m) ^ ((m) << 32)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt << 5);                           \
                                                         \
        nt = (uint32_t)((m) >> 48);                      \
        nt = (nt | -nt) >> 31;                           \
        (m) ^= ((m) ^ ((m) << 16)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt << 4);                           \
                                                         \
        nt = (uint32_t)((m) >> 56);                      \
        nt = (nt | -nt) >> 31;                           \
        (m) ^= ((m) ^ ((m) << 8)) & ((uint64_t)nt - 1);  \
        (e) += (int)(nt << 3);                           \
                                                         \
        nt = (uint32_t)((m) >> 60);                      \
        nt = (nt | -nt) >> 31;                           \
        (m) ^= ((m) ^ ((m) << 4)) & ((uint64_t)nt - 1);  \
        (e) += (int)(nt << 2);                           \
                                                         \
        nt = (uint32_t)((m) >> 62);                      \
        nt = (nt | -nt) >> 31;                           \
        (m) ^= ((m) ^ ((m) << 2)) & ((uint64_t)nt - 1);  \
        (e) += (int)(nt << 1);                           \
                                                         \
        nt = (uint32_t)((m) >> 63);                      \
        (m) ^= ((m) ^ ((m) << 1)) & ((uint64_t)nt - 1);  \
        (e) += (int)(nt);                                \
    } while (0)

/* Keep only this typedef (as you requested). */
typedef uint64_t fpr;

static const fpr fpr_inv_2sqrsigma0 = 4594603506513722306;
static const fpr fpr_log2 = 4604418534313441775;
static const fpr fpr_inv_log2 = 4609176140021203710;
static const fpr fpr_ptwo63 = 4890909195324358656;

/* --------- Catapult-safe byte load/store helpers (NO pointer casts) --------- */
static inline uint32_t load_u32_le(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static inline uint64_t load_u64_le(const uint8_t *p)
{
    return (uint64_t)p[0] | ((uint64_t)p[1] << 8) | ((uint64_t)p[2] << 16) | ((uint64_t)p[3] << 24) | ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) | ((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}

static inline void store_u64_le(uint8_t *p, uint64_t x)
{
    p[0] = (uint8_t)x;
    p[1] = (uint8_t)(x >> 8);
    p[2] = (uint8_t)(x >> 16);
    p[3] = (uint8_t)(x >> 24);
    p[4] = (uint8_t)(x >> 32);
    p[5] = (uint8_t)(x >> 40);
    p[6] = (uint8_t)(x >> 48);
    p[7] = (uint8_t)(x >> 56);
}
/* -------------------------------------------------------------------------- */

static inline int64_t
fpr_irsh(int64_t x, int n)
{
    x ^= (x ^ (x >> 32)) & -(int64_t)(n >> 5);
    return x >> (n & 31);
}

static inline int64_t
fpr_floor(fpr x)
{
    uint64_t t;
    int64_t xi;
    int e, cc;

    e = (int)(x >> 52) & 0x7FF;
    t = x >> 63;
    xi = (int64_t)(((x << 10) | ((uint64_t)1 << 62)) & (((uint64_t)1 << 63) - 1));
    xi = (xi ^ -(int64_t)t) + (int64_t)t;
    cc = 1085 - e;

    xi = fpr_irsh(xi, cc & 63);

    xi ^= (xi ^ -(int64_t)t) & -(int64_t)((uint32_t)(63 - cc) >> 31);
    return xi;
}

static inline uint64_t
fpr_ulsh(uint64_t x, int n)
{
    x ^= (x ^ (x << 32)) & -(uint64_t)(n >> 5);
    return x << (n & 31);
}

static inline uint64_t
fpr_ursh(uint64_t x, int n)
{
    x ^= (x ^ (x >> 32)) & -(uint64_t)(n >> 5);
    return x >> (n & 31);
}

static inline fpr
FPR(int s, int e, uint64_t m)
{
    fpr x;
    uint32_t t;
    unsigned f;

    e += 1076;
    t = (uint32_t)e >> 31;
    m &= (uint64_t)t - 1;

    t = (uint32_t)(m >> 54);
    e &= -(int)t;

    x = (((uint64_t)s << 63) | (m >> 2)) + ((uint64_t)(uint32_t)e << 52);

    f = (unsigned)m & 7U;
    x += (0xC8U >> f) & 1;
    return x;
}

fpr fpr_add(fpr x, fpr y)
{
    uint64_t m, xu, yu, za;
    uint32_t cs;
    int ex, ey, sx, sy, cc;

    m = ((uint64_t)1 << 63) - 1;
    za = (x & m) - (y & m);
    cs = (uint32_t)(za >> 63) | ((1U - (uint32_t)(-za >> 63)) & (uint32_t)(x >> 63));
    m = (x ^ y) & -(uint64_t)cs;
    x ^= m;
    y ^= m;

    ex = (int)(x >> 52);
    sx = ex >> 11;
    ex &= 0x7FF;
    m = (uint64_t)(uint32_t)((ex + 0x7FF) >> 11) << 52;
    xu = ((x & (((uint64_t)1 << 52) - 1)) | m) << 3;
    ex -= 1078;

    ey = (int)(y >> 52);
    sy = ey >> 11;
    ey &= 0x7FF;
    m = (uint64_t)(uint32_t)((ey + 0x7FF) >> 11) << 52;
    yu = ((y & (((uint64_t)1 << 52) - 1)) | m) << 3;
    ey -= 1078;

    cc = ex - ey;
    yu &= -(uint64_t)((uint32_t)(cc - 60) >> 31);
    cc &= 63;

    m = fpr_ulsh(1, cc) - 1;
    yu |= (yu & m) + m;
    yu = fpr_ursh(yu, cc);

    xu += yu - ((yu << 1) & -(uint64_t)(sx ^ sy));

    FPR_NORM64(xu, ex);

    xu |= ((uint32_t)xu & 0x1FF) + 0x1FF;
    xu >>= 9;
    ex += 9;

    return FPR(sx, ex, xu);
}

static inline fpr
fpr_sub(fpr x, fpr y)
{
    y ^= (uint64_t)1 << 63;
    return fpr_add(x, y);
}

fpr fpr_scaled(int64_t i, int sc)
{
    int s, e;
    uint32_t t;
    uint64_t m;

    s = (int)((uint64_t)i >> 63);
    i ^= -(int64_t)s;
    i += s;

    m = (uint64_t)i;
    e = 9 + sc;
    FPR_NORM64(m, e);

    m |= ((uint32_t)m & 0x1FF) + 0x1FF;
    m >>= 9;

    t = (uint32_t)((uint64_t)(i | -i) >> 63);
    m &= -(uint64_t)t;
    e &= -(int)t;

    return FPR(s, e, m);
}

static inline fpr
fpr_of(int64_t i)
{
    return fpr_scaled(i, 0);
}

fpr fpr_mul(fpr x, fpr y)
{
    uint64_t xu, yu, w, zu, zv;
    uint32_t x0, x1, y0, y1, z0, z1, z2;
    int ex, ey, d, e, s;

    xu = (x & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);
    yu = (y & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);

    x0 = (uint32_t)xu & 0x01FFFFFF;
    x1 = (uint32_t)(xu >> 25);
    y0 = (uint32_t)yu & 0x01FFFFFF;
    y1 = (uint32_t)(yu >> 25);

    w = (uint64_t)x0 * (uint64_t)y0;
    z0 = (uint32_t)w & 0x01FFFFFF;
    z1 = (uint32_t)(w >> 25);

    w = (uint64_t)x0 * (uint64_t)y1;
    z1 += (uint32_t)w & 0x01FFFFFF;
    z2 = (uint32_t)(w >> 25);

    w = (uint64_t)x1 * (uint64_t)y0;
    z1 += (uint32_t)w & 0x01FFFFFF;
    z2 += (uint32_t)(w >> 25);

    zu = (uint64_t)x1 * (uint64_t)y1;
    z2 += (z1 >> 25);
    z1 &= 0x01FFFFFF;
    zu += z2;

    zu |= ((z0 | z1) + 0x01FFFFFF) >> 25;

    zv = (zu >> 1) | (zu & 1);
    w = zu >> 55;
    zu ^= (zu ^ zv) & -w;

    ex = (int)((x >> 52) & 0x7FF);
    ey = (int)((y >> 52) & 0x7FF);
    e = ex + ey - 2100 + (int)w;

    s = (int)((x ^ y) >> 63);

    d = ((ex + 0x7FF) & (ey + 0x7FF)) >> 11;
    zu &= -(uint64_t)d;

    return FPR(s, e, zu);
}

static inline fpr fpr_sqr(fpr x) { return fpr_mul(x, x); }

static inline fpr
fpr_half(fpr x)
{
    uint32_t t;

    x -= (uint64_t)1 << 52;
    t = (((uint32_t)(x >> 52) & 0x7FF) + 1) >> 11;
    x &= (uint64_t)t - 1;
    return x;
}

void PQCLEAN_FALCONPADDED512_CLEAN_prng_refill(uint8_t buf[512], size_t *ptr, uint8_t state_bytes[256])
{
    static const uint32_t CW[] = {
        0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};

    uint64_t cc;
    size_t u;

    /* Catapult-safe: no (uint64_t*) cast */
    cc = load_u64_le(state_bytes + 48);

    for (u = 0; u < 8; u++)
    {
        uint32_t state[16];
        size_t v;
        int i;

        /* Replace memcpy(&state[0], CW, sizeof CW); */
        state[0] = CW[0];
        state[1] = CW[1];
        state[2] = CW[2];
        state[3] = CW[3];

        /* Replace memcpy(&state[4], state_bytes, 48); (12 words = 48 bytes) */
        for (v = 0; v < 12; v++)
        {
            state[4 + v] = load_u32_le(state_bytes + 4u * (uint32_t)v);
        }

        state[14] ^= (uint32_t)cc;
        state[15] ^= (uint32_t)(cc >> 32);

        for (i = 0; i < 10; i++)
        {
#define QROUND(a, b, c, d)                              \
    do                                                  \
    {                                                   \
        state[a] += state[b];                           \
        state[d] ^= state[a];                           \
        state[d] = (state[d] << 16) | (state[d] >> 16); \
        state[c] += state[d];                           \
        state[b] ^= state[c];                           \
        state[b] = (state[b] << 12) | (state[b] >> 20); \
        state[a] += state[b];                           \
        state[d] ^= state[a];                           \
        state[d] = (state[d] << 8) | (state[d] >> 24);  \
        state[c] += state[d];                           \
        state[b] ^= state[c];                           \
        state[b] = (state[b] << 7) | (state[b] >> 25);  \
    } while (0)

            QROUND(0, 4, 8, 12);
            QROUND(1, 5, 9, 13);
            QROUND(2, 6, 10, 14);
            QROUND(3, 7, 11, 15);
            QROUND(0, 5, 10, 15);
            QROUND(1, 6, 11, 12);
            QROUND(2, 7, 8, 13);
            QROUND(3, 4, 9, 14);

#undef QROUND
        }

        for (v = 0; v < 4; v++)
        {
            state[v] += CW[v];
        }

        /* Catapult-safe: no (uint32_t*) cast */
        for (v = 4; v < 14; v++)
        {
            state[v] += load_u32_le(state_bytes + 4u * (uint32_t)(v - 4u));
        }
        state[14] += load_u32_le(state_bytes + 4u * 10u) ^ (uint32_t)cc;
        state[15] += load_u32_le(state_bytes + 4u * 11u) ^ (uint32_t)(cc >> 32);

        cc++;

        for (v = 0; v < 16; v++)
        {
            buf[(u << 2) + (v << 5) + 0] = (uint8_t)state[v];
            buf[(u << 2) + (v << 5) + 1] = (uint8_t)(state[v] >> 8);
            buf[(u << 2) + (v << 5) + 2] = (uint8_t)(state[v] >> 16);
            buf[(u << 2) + (v << 5) + 3] = (uint8_t)(state[v] >> 24);
        }
    }

    /* Catapult-safe: no (uint64_t*) cast */
    store_u64_le(state_bytes + 48, cc);

    *ptr = 0;
}

static inline uint64_t
prng_get_u64(uint8_t buf[512], size_t *ptr, uint8_t state_bytes[256])
{
    size_t u = *ptr;

    /* Catapult-safe: no compound literal sizeof trick */
    if (u >= (512u - 9u))
    {
        PQCLEAN_FALCONPADDED512_CLEAN_prng_refill(buf, ptr, state_bytes);
        u = 0;
    }
    *ptr = u + 8;

    return (uint64_t)buf[u + 0] |
           ((uint64_t)buf[u + 1] << 8) |
           ((uint64_t)buf[u + 2] << 16) |
           ((uint64_t)buf[u + 3] << 24) |
           ((uint64_t)buf[u + 4] << 32) |
           ((uint64_t)buf[u + 5] << 40) |
           ((uint64_t)buf[u + 6] << 48) |
           ((uint64_t)buf[u + 7] << 56);
}

static inline unsigned
prng_get_u8(uint8_t buf[512], size_t *ptr, uint8_t state_bytes[256])
{
    // (void)state_bytes; /* unused, but keep signature consistent */
    unsigned v = buf[(*ptr)++];
    if (*ptr == 512u)
    {
        PQCLEAN_FALCONPADDED512_CLEAN_prng_refill(buf, ptr, state_bytes);
    }
    return v;
}

int PQCLEAN_FALCONPADDED512_CLEAN_gaussian0_sampler(uint8_t buf[512], size_t *ptr, uint8_t state_bytes[256])
{
    static const uint32_t dist[] = {
        10745844u, 3068844u, 3741698u,
        5559083u, 1580863u, 8248194u,
        2260429u, 13669192u, 2736639u,
        708981u, 4421575u, 10046180u,
        169348u, 7122675u, 4136815u,
        30538u, 13063405u, 7650655u,
        4132u, 14505003u, 7826148u,
        417u, 16768101u, 11363290u,
        31u, 8444042u, 8086568u,
        1u, 12844466u, 265321u,
        0u, 1232676u, 13644283u,
        0u, 38047u, 9111839u,
        0u, 870u, 6138264u,
        0u, 14u, 12545723u,
        0u, 0u, 3104126u,
        0u, 0u, 28824u,
        0u, 0u, 198u,
        0u, 0u, 1u};

    uint32_t v0, v1, v2, hi;
    uint64_t lo;
    size_t u;
    int z;

    lo = prng_get_u64(buf, ptr, state_bytes);
    hi = prng_get_u8(buf, ptr, state_bytes);
    v0 = (uint32_t)lo & 0xFFFFFF;
    v1 = (uint32_t)(lo >> 24) & 0xFFFFFF;
    v2 = (uint32_t)(lo >> 48) | (hi << 16);

    z = 0;
    for (u = 0; u < (sizeof dist) / sizeof(dist[0]); u += 3)
    {
        uint32_t w0, w1, w2, cc;

        w0 = dist[u + 2];
        w1 = dist[u + 1];
        w2 = dist[u + 0];
        cc = (v0 - w0) >> 31;
        cc = (v1 - w1 - cc) >> 31;
        cc = (v2 - w2 - cc) >> 31;
        z += (int)cc;
    }
    return z;
}

static inline int64_t
fpr_trunc(fpr x)
{
    uint64_t t, xu;
    int e, cc;

    e = (int)(x >> 52) & 0x7FF;
    xu = ((x << 10) | ((uint64_t)1 << 62)) & (((uint64_t)1 << 63) - 1);
    cc = 1085 - e;
    xu = fpr_ursh(xu, cc & 63);

    xu &= -(uint64_t)((uint32_t)(cc - 64) >> 31);

    t = x >> 63;
    xu = (xu ^ -t) + t;

    /* Catapult-safe: avoid *(int64_t*)&xu */
    return (int64_t)xu;
}

uint64_t
fpr_expm_p63(fpr x, fpr ccs)
{
    static const uint64_t C[] = {
        0x00000004741183A3u,
        0x00000036548CFC06u,
        0x0000024FDCBF140Au,
        0x0000171D939DE045u,
        0x0000D00CF58F6F84u,
        0x000680681CF796E3u,
        0x002D82D8305B0FEAu,
        0x011111110E066FD0u,
        0x0555555555070F00u,
        0x155555555581FF00u,
        0x400000000002B400u,
        0x7FFFFFFFFFFF4800u,
        0x8000000000000000u};

    uint64_t z, y;
    unsigned u;
    uint32_t z0, z1, y0, y1;
    uint64_t a, b;

    y = C[0];
    z = (uint64_t)fpr_trunc(fpr_mul(x, fpr_ptwo63)) << 1;
    for (u = 1; u < (sizeof C) / sizeof(C[0]); u++)
    {
        uint64_t c;

        z0 = (uint32_t)z;
        z1 = (uint32_t)(z >> 32);
        y0 = (uint32_t)y;
        y1 = (uint32_t)(y >> 32);
        a = ((uint64_t)z0 * (uint64_t)y1) + (((uint64_t)z0 * (uint64_t)y0) >> 32);
        b = ((uint64_t)z1 * (uint64_t)y0);
        c = (a >> 32) + (b >> 32);
        c += (((uint64_t)(uint32_t)a + (uint64_t)(uint32_t)b) >> 32);
        c += (uint64_t)z1 * (uint64_t)y1;
        y = C[u] - c;
    }

    z = (uint64_t)fpr_trunc(fpr_mul(ccs, fpr_ptwo63)) << 1;
    z0 = (uint32_t)z;
    z1 = (uint32_t)(z >> 32);
    y0 = (uint32_t)y;
    y1 = (uint32_t)(y >> 32);
    a = ((uint64_t)z0 * (uint64_t)y1) + (((uint64_t)z0 * (uint64_t)y0) >> 32);
    b = ((uint64_t)z1 * (uint64_t)y0);
    y = (a >> 32) + (b >> 32);
    y += (((uint64_t)(uint32_t)a + (uint64_t)(uint32_t)b) >> 32);
    y += (uint64_t)z1 * (uint64_t)y1;

    return y;
}

static int
BerExp(uint8_t buf[512], size_t *ptr, uint8_t state_bytes[256], fpr x, fpr ccs)
{
    int s, i;
    fpr r;
    uint32_t sw, w;
    uint64_t z;

    s = (int)fpr_trunc(fpr_mul(x, fpr_inv_log2));
    r = fpr_sub(x, fpr_mul(fpr_of(s), fpr_log2));

    sw = (uint32_t)s;
    sw ^= (sw ^ 63) & -((63 - sw) >> 31);
    s = (int)sw;

    z = ((fpr_expm_p63(r, ccs) << 1) - 1) >> s;

    i = 64;
    do
    {
        i -= 8;
        w = prng_get_u8(buf, ptr, state_bytes) - ((uint32_t)(z >> i) & 0xFF);
    } while (!w && i > 0);
    return (int)(w >> 31);
}

int sampler(uint8_t prng_buf[512], size_t *prng_ptr, uint8_t prng_state[256],
            fpr sigma_min, fpr mu, fpr isigma)
{
    int s;
    fpr r, dss, ccs;

    s = (int)fpr_floor(mu);
    r = fpr_sub(mu, fpr_of(s));

    dss = fpr_half(fpr_sqr(isigma));

    ccs = fpr_mul(isigma, sigma_min);

    for (;;)
    {
        int z0, z, b;
        fpr x;

        z0 = PQCLEAN_FALCONPADDED512_CLEAN_gaussian0_sampler(prng_buf, prng_ptr, prng_state);
        b = (int)prng_get_u8(prng_buf, prng_ptr, prng_state) & 1;
        z = b + ((b << 1) - 1) * z0;

        x = fpr_mul(fpr_sqr(fpr_sub(fpr_of(z), r)), dss);
        x = fpr_sub(x, fpr_mul(fpr_of(z0 * z0), fpr_inv_2sqrsigma0));
        if (BerExp(prng_buf, prng_ptr, prng_state, x, ccs))
        {
            return s + z;
        }
    }
}

int main(void)
{
    uint8_t prng_buf[512];
    size_t prng_ptr = 0;
    uint8_t prng_state[256];
    fpr sigma_min;

    fpr mu, isigma;
    int out;

    memset(prng_buf, 0, sizeof prng_buf);
    memset(prng_state, 0, sizeof prng_state);

    for (size_t i = 0; i < 48; i++)
    {
        prng_state[i] = (uint8_t)(0xA5u + (uint8_t)i);
    }

    /* Catapult-safe: no (uint64_t*) cast */
    store_u64_le(prng_state + 48, 1);

    PQCLEAN_FALCONPADDED512_CLEAN_prng_refill(prng_buf, &prng_ptr, prng_state);

    sigma_min = fpr_of(12) / fpr_of(10); /* 1.2 */

    mu = fpr_of(3) + fpr_of(1) / fpr_of(4); /* 3.25 */
    isigma = fpr_of(1);

    out = sampler(prng_buf, &prng_ptr, prng_state, sigma_min, mu, isigma);
    printf("sampler(mu=3.25, isigma=1.0) -> %d\n", out);

    return 0;
}

/**
 * unified_butterfly_unit.c
 *
 * Refactored NTT with a Unified Butterfly/Multiplier ALU.
 *
 * Architecture: Single-Multiplier Resource with Branchless Input Multiplexing
 * Target:       FPGA/ASIC synthesis (Kyber / ML-KEM NTT core)
 * Author note:  All mode selection is performed via bitwise masking BEFORE
 *               the Montgomery multiplier, guaranteeing exactly ONE multiply
 *               path through the datapath regardless of mode.
 */

#include <stdint.h>
#include <stdio.h>

/* =========================================================================
 * Domain Constants
 * ========================================================================= */
#define N 256
#define Q 3329
#define QINV (-3327) /* Q^{-1} mod 2^16, i.e. Q * QINV ≡ -1 (mod 2^16) */

/* =========================================================================
 * Mode Encoding
 *   MODE_BUTTERFLY  (0) – Full radix-2 DIT butterfly
 *   MODE_MUL_ONLY   (1) – Standalone modular multiplier (passes a_in * b_in)
 * ========================================================================= */
#define MODE_BUTTERFLY 0u
#define MODE_MUL_ONLY 1u

/* =========================================================================
 * Twiddle-Factor Table  (zetas[k], k = 1..128)
 * ========================================================================= */
static const int16_t zetas[128] = {
    -1044, -758, -359, -1517, 1493, 1422, 287, 202,
    -171, 622, 1577, 182, 962, -1202, -1474, 1468,
    573, -1325, 264, 383, -829, 1458, -1602, -130,
    -681, 1017, 732, 608, -1542, 411, -205, -1571,
    1223, 652, -552, 1015, -1293, 1491, -282, -1544,
    516, -8, -320, -666, -1618, -1162, 126, 1469,
    -853, -90, -271, 830, 107, -1421, -247, -951,
    -398, 961, -1508, -725, 448, -1065, 677, -1275,
    -1103, 430, 555, 843, -1251, 871, 1550, 105,
    422, 587, 177, -235, -291, -460, 1574, 1653,
    -246, 778, 1159, -147, -777, 1483, -602, 1119,
    -1590, 644, -872, 349, 418, 329, -156, -75,
    817, 1097, 603, 610, 1322, -1285, -1465, 384,
    -1215, -136, 1218, -1335, -874, 220, -1187, -1659,
    -1185, -1530, -1278, 794, -1510, -854, -870, 478,
    -108, -308, 996, 991, 958, -1460, 1522, 1628};

/* =========================================================================
 * Result type returned by the Unified Butterfly Unit
 *
 *   Mode 0 (BUTTERFLY):
 *       out_lower  =  lower + t       where t = MontMul(zeta, upper)
 *       out_upper  =  lower - t
 *
 *   Mode 1 (MUL_ONLY):
 *       out_lower  =  MontMul(a_in, b_in)
 *       out_upper  =  0               (architecturally unused; tied low)
 * ========================================================================= */
typedef struct
{
    int16_t out_lower;
    int16_t out_upper;
} UBUResult;

/* =========================================================================
 * Montgomery Reduction  — the SINGLE physical multiplier resource
 *
 *   Computes  a * R^{-1}  (mod Q),  R = 2^16
 *   Input range: a ∈ [-Q·2^15 , Q·2^15)
 * ========================================================================= */
static inline int16_t montgomery_reduce(int32_t a)
{
    /*
     * Step 1: Compute the low-word correction factor.
     *         Cast truncates to the 16 LSBs (mod 2^16).
     */
    int32_t t = (int16_t)((int16_t)a * QINV);

    /*
     * Step 2: Subtract and right-shift to complete the reduction.
     *         u >> 16 discards the lower 16 bits, yielding the
     *         Montgomery-reduced result in the high word.
     */
    int32_t u = a - t * (int32_t)Q;
    return (int16_t)(u >> 16);
}

/* =========================================================================
 * fqmul — thin wrapper, kept for readability/auditability
 * ========================================================================= */
static inline int16_t fqmul(int16_t a, int16_t b)
{
    return montgomery_reduce((int32_t)a * (int32_t)b);
}

/* =========================================================================
 * unified_butterfly_unit  (UBU)
 *
 * Hardware mapping intent
 * -----------------------
 * This function is the entire arithmetic datapath for one butterfly slot.
 * Synthesis tools targeting FPGA/ASIC will see:
 *
 *   [Mode Mux] → [ONE DSP / Multiplier] → [Adder/Subtractor] → [Out Mux]
 *
 * The branchless mask-based mux BEFORE the multiplier guarantees that
 * exactly one operand pair is presented to the physical multiplier per
 * call.  The "compute-both-then-mask" anti-pattern is deliberately avoided;
 * that pattern still synthesises two multipliers and multiplexes the
 * output, wasting area and power.
 *
 * Parameters
 * ----------
 *   mode   : MODE_BUTTERFLY (0) or MODE_MUL_ONLY (1)
 *   lower  : r[j]       — lower butterfly arm     (Mode 0 only)
 *   upper  : r[j+len]   — upper butterfly arm     (Mode 0 only)
 *   zeta   : twiddle factor                       (Mode 0 only)
 *   a_in   : multiplicand                         (Mode 1 only)
 *   b_in   : multiplier                           (Mode 1 only)
 *
 * Constant-time / side-channel note
 * ----------------------------------
 * All branching on `mode` is eliminated.  The derivation of `mask` from
 * the mode bit and its application via bitwise AND/OR constitutes a
 * data-oblivious MUX: the same sequence of operations executes for every
 * call, with only the data values (not the control flow) varying.
 * ========================================================================= */
static inline UBUResult unified_butterfly_unit(
    uint8_t mode,
    int16_t lower,
    int16_t upper,
    int16_t zeta,
    int16_t a_in,
    int16_t b_in)
{
    UBUResult result;

    /*
     * ── STAGE 1 : INPUT MULTIPLEXING (branchless) ──────────────────────────
     *
     * Derive a 16-bit all-ones mask from the LSB of `mode`:
     *
     *   mode == MODE_BUTTERFLY (0) → mask = 0x0000  (select zeta / upper)
     *   mode == MODE_MUL_ONLY  (1) → mask = 0xFFFF  (select a_in / b_in)
     *
     * Using negation of the zero-extended mode bit exploits two's-complement
     * arithmetic to fill all bits from one flag, a standard hardware idiom
     * (equivalent to FPGA LUT-based MUX primitives when synthesised).
     */
    const int16_t mask = -(int16_t)(mode & 1u); /* 0x0000 or 0xFFFF */
    const int16_t imask = ~mask;                /* complement        */

    /*
     * Mux operand A: zeta  (Mode 0) or a_in (Mode 1)
     * Mux operand B: upper (Mode 0) or b_in (Mode 1)
     *
     * Bitwise:  selected = (mode0_val & imask) | (mode1_val & mask)
     *
     * When mask == 0x0000 (butterfly):
     *   sel_a = (zeta  & 0xFFFF) | (a_in & 0x0000) = zeta
     *   sel_b = (upper & 0xFFFF) | (b_in & 0x0000) = upper
     *
     * When mask == 0xFFFF (mul_only):
     *   sel_a = (zeta  & 0x0000) | (a_in & 0xFFFF) = a_in
     *   sel_b = (upper & 0x0000) | (b_in & 0xFFFF) = b_in
     */
    const int16_t sel_a = (zeta & imask) | (a_in & mask);
    const int16_t sel_b = (upper & imask) | (b_in & mask);

    /*
     * ── STAGE 2 : SINGLE MONTGOMERY MULTIPLICATION ─────────────────────────
     *
     * Exactly ONE call to fqmul, fed by the muxed inputs above.
     * In physical synthesis this maps to a single DSP slice or dedicated
     * multiplier cell — no duplication, no dead computation.
     */
    const int16_t t = fqmul(sel_a, sel_b);

    /*
     * ── STAGE 3 : BUTTERFLY ARITHMETIC ─────────────────────────────────────
     *
     * Compute both butterfly output candidates unconditionally.
     * These are adder/subtractor cells — negligible area vs. a multiplier.
     * The cost of computing both is therefore architecturally acceptable.
     *
     *   bf_lower = lower + t   (the "sum"  arm of the butterfly)
     *   bf_upper = lower - t   (the "diff" arm of the butterfly)
     */
    const int16_t bf_lower = lower + t;
    const int16_t bf_upper = lower - t;

    /*
     * ── STAGE 4 : OUTPUT MULTIPLEXING (branchless) ─────────────────────────
     *
     * Mode 0 (butterfly):  present bf_lower / bf_upper to caller.
     * Mode 1 (mul_only):   present t / 0 — the raw product and a tied-low
     *                      upper word (architecturally unused by the caller).
     *
     * Same mask-and-OR idiom as Stage 1.
     */
    result.out_lower = (bf_lower & imask) | (t & mask);
    result.out_upper = (bf_upper & imask) | ((int16_t)0 & mask);

    return result;
}

/* =========================================================================
 * ntt — Number Theoretic Transform (Cooley-Tukey, DIT, radix-2)
 *
 * Calls unified_butterfly_unit in MODE_BUTTERFLY for every butterfly.
 * The call site is a drop-in replacement for the original inline
 * fqmul + add/sub triplet, preserving bit-exact mathematical equivalence.
 * ========================================================================= */
void ntt(int16_t r[N])
{
    unsigned int len, start, j;
    unsigned int k = 1; /* twiddle-factor index */
    int16_t zeta;
    UBUResult res;

    /* Cooley-Tukey decimation-in-time: stages from len=128 down to len=2 */
    for (len = 128; len >= 2; len >>= 1)
    {
        for (start = 0; start < N; start = j + len)
        {
            zeta = zetas[k++];

            for (j = start; j < start + len; j++)
            {
                /*
                 * One UBU call replaces:
                 *   t          = fqmul(zeta, r[j + len]);
                 *   r[j + len] = r[j] - t;
                 *   r[j]       = r[j] + t;
                 *
                 * a_in / b_in are architecturally irrelevant in MODE_BUTTERFLY;
                 * passing 0 keeps synthesis tools from inferring latches and
                 * makes the dead inputs explicit.
                 */
                res = unified_butterfly_unit(
                    MODE_BUTTERFLY,
                    r[j],       /* lower arm            */
                    r[j + len], /* upper arm            */
                    zeta,       /* twiddle factor       */
                    (int16_t)0, /* a_in — unused        */
                    (int16_t)0  /* b_in — unused        */
                );

                r[j] = res.out_lower;
                r[j + len] = res.out_upper;
            }
        }
    }
}

/* =========================================================================
 * Optional standalone multiplier demo
 * Shows MODE_MUL_ONLY driving the same physical datapath.
 * ========================================================================= */
int16_t modular_multiply(int16_t a, int16_t b)
{
    UBUResult res = unified_butterfly_unit(
        MODE_MUL_ONLY,
        (int16_t)0, /* lower — unused */
        (int16_t)0, /* upper — unused */
        (int16_t)0, /* zeta  — unused */
        a,
        b);
    return res.out_lower; /* t = MontMul(a, b) */
}

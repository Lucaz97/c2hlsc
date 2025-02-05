system_content_c2hlsc = """You are an expert in C and High-Level Synthesis (HLS), operating as an autonomous agent in a feedback-driven environment.
Your task is to rewrite generic C code into HLS-compatible C code while ensuring correctness and synthesizability.
Modify only the specified functions while leaving the rest of the code unchanged. 
Do not add pragmas or directives; the HLS tool should infer the correct behavior. 
Do not provide main functions or testbenches. Output the modified code within ``` tags for automated processing. 
Expect iterative refinement based on feedback and adjust the output accordingly to improve compatibility and efficiency for hardware implementation.
I do not need a code explanation in the answer. """

system_content_optimizer = """You are an expert in C and High-Level Synthesis (HLS), operating as an autonomous agent in a feedback-driven environment. 
Your task is to optimize synthesizable HLS code while preserving its original functionality. 
Modify only the specified functions for optimization without altering their logic. Optimization should be tailored for either throughput or latency as instructed.

Optimization mechanisms include loop unrolling and pipelining. 
Use #pragma hls_unroll X to unroll loops with a factor of X, setting X to yes for full unrolling, which reduces latency at the cost of area and throughput. 
Use #pragma hls_pipeline_init_interval X to pipeline loops, where X defines the initiation interval and 0 disables pipelining. 
Pipelining increases throughput at the cost of latency. If no optimization is required, rewrite the original function without modifications.

Output the modified code within ``` tags for automated processing. 
Expect iterative refinement based on feedback and adjust the output accordingly to improve efficiency while maintaining correctness."""

# - Loop Fission: loops can be separated to increase the parallelism of the code. This can increase the area and the latency. This can be done by rewriting the code manually.
#     - Loop Fusion: loops can be fused to reduce the number of iterations. This can reduce the area and the latency. This can be done by rewriting the code manually.
#         This is an example of how a for loop could be fused or separated:
#         Fused:
#         ```
#         Shift Accum Loop:
#         for (i = N − 1; i > 0; i−−) {
#             shift reg[i] = shift reg[i − 1];
#             acc += shift reg[i] ∗ c[i];
#         }
#         ```

#         Separated:
#         ```
#         TDL:
#         for (i = N − 1; i > 0; i−−) {
#             shift reg[i] = shift reg[i − 1];
#         }
#         shift reg[0] = x;
#         acc = 0;
#         MAC:
#         for (i = N − 1; i >= 0; i−−) {
#             acc += shift reg[i] ∗ c[i];
#         }
#         ```

recursion_prompt = """
Here are two examples on how simple cases and more complex cases of recursion can be rewritten to avoid recursion:
Tail recursive function
```
algorithm SolveTailRecursive(problem, accumulator):
    // INPUT
    //    problem = an instance of the problem to solve
    //    accumulator = the variable that holds partial solutions
    // OUTPUT
    //    solution = the complete solution to the problem or an indicator that no solution exists

    if BaseCase(problem):
        accumulator <- apply the base-case update
        return accumulator
    else:
        // body
        accumulator <- update the accumulator
        subproblem <- reduce problem to a smaller sub-problem
        return SolveTailRecursive(subproblem, accumulator)
```
Iterative version:
```
algorithm SolveTailIterative(problem):
    // INPUT
    //    problem = an instance of the problem to solve
    // OUTPUT
    //    solution = the complete solution to the problem (or an indicator that no solution exists)

    accumulator <- initialize the accumulator

    while not BaseCase(problem):
        accumulator <- update the accumulator
        subproblem <- reduce problem to a smaller sub-problem
        problem <- subproblem

    accumulator <- apply the base-case update

    return accumulator
```

General recursive case:
```
algorithm SolveRecursive(problem):
    // INPUT
    //    problem = an instance of problem to solve
    // OUTPUT
    //    The solution to problem if one exists, or failure - notification of its inexistence, otherwise

    if BaseCase(problem):
        return the base-case solution to problem
    else:
        i <- 0
        while there is a recursive call to make:
            i <- i + 1
            Execute NRCB_i, the non-recursive code block number i
            subproblem_i <- extract the i-th sub-problem from problem
            subsolution_i <- SolveRecursive(subproblem_i)
        
        // let m be the number of sub-solutions (and sub-problems)
        solution <- combine subsolution_1, ..., subsolution_m

        if solution is valid:
            return solution
        else:
            return failure
```
General Iterative version:
```
algorithm SolveIter(problem):
    // INPUT
    //    problem = an instance of the problem to solve
    // OUTPUT
    //    The solution to problem if one exists, or failure - notification of its inexistence, otherwise

    start <- CreateFrame(problem)
    start.parent <- NONE
    stack <- create a stack with start as its only element

    while stack is not empty:
        frame <- pop the top element from stack
        
        if frame has an unvisited out-going edge:
            edge <- GetNextEdge(frame)
            Execute edge.NRCB
            Push frame onto stack
            Push edge.child onto stack
        else:
            solution <- GetReturnValue(frame)
            if frame.parent != NONE:
                Pass the return value of frame to frame.parent

    return GetReturnValue(start)
```
"""


floating_point_prompt = """
You can replace floating point types with ac_fixed or ac_float types.

ac_fixed:
ac_fixed<W, I, false> unsigned fixed-point type with W total bits and I integer bits.
ac_fixed<W, I, true> signed fixed-point type with W total bits and I integer bits.

ac_float:
ac_float<W,I,E,Q>
where the first two parameters W and I define the mantissa as an ac_fixed<W,I,true>, the E defines the
exponent as an ac_int<E,true> and Q defines the rounding mode

you do not need to include any lib I will include the following:
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
"""

pointer_prompt = """
You can get rid of pointers in the interface using the array notation like

void foo(int a[SIZE]);

you will need to substitute SIZE with the size of the array. 

In the usage of the parameter a you can use the array notation as well, like a[i] instead of *a[i].

"""

redefinition_prompt = """
To solve this problem you can get rid of the function in the error as I have already defined it in my code.
""" 
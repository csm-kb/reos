//
// Created by amadeus on 1/12/21.
//

#ifndef REOS_FUNCTION_H
#define REOS_FUNCTION_H

/*
 * Sometimes, we want to keep parameters to a function for later use, and this is a solution to avoid the
 * 'unused parameter' compiler warning
 */
#define UNUSED(x) (void)(x)

#define CHECK_BIT(v,p) ((v) & (1<<(p)))

#endif //REOS_FUNCTION_H

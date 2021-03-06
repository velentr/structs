/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute
 * this software, either in source code form or as a compiled binary, for any
 * purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors of this
 * software dedicate any and all copyright interest in the software to the
 * public domain. We make this dedication for the benefit of the public at large
 * and to the detriment of our heirs and successors. We intend this dedication
 * to be an overt act of relinquishment in perpetuity of all present and future
 * rights to this software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

/**
 * \file kmp.c
 *
 * \brief Knuth-Morris-Pratt string searching algorithm.
 *
 * The KMP string searching algorithm attempts to locate a given \c needle
 * within the given \c haystack. The for a \c needle of length \c k and a \c
 * haystack of length \c n, KMP runs in O(n + k) time and uses O(k) space.
 *
 * \author Brian Kubisiak
 */

#include <assert.h>
#include <stdlib.h>

#include "kmp.h"

/**
 * \brief Compute the backtracking table for KMP.
 *
 * \param [in] key The key for which the table is computed.
 * \param [out] table Memory for storing the computed table.
 * \param [in] len Length of the key (and the resulting table).
 *
 * \note The time complexity of this function is O(len).
 */
static void _compute_table(const char *key, size_t *table, size_t len)
{
    size_t pos;
    size_t cnd;

    pos = 2;
    cnd = 0;

    assert(len > 0);

    switch (len)
    {
    case 2:
        table[1] = (size_t)0;
        /* fallthrough */
    case 1:
        table[0] = (size_t)~0;
        return;
    default:
        table[0] = (size_t)~0;
        table[1] = (size_t)0;
    }

    assert(len > 2);

    while (pos < len)
    {
        if (key[pos-1] == key[cnd])
        {
            table[pos] = cnd + 1;
            cnd++;
            pos++;
        }
        else if (cnd > 0)
        {
            cnd = table[cnd];
        }
        else
        {
            table[pos] = 0;
            pos++;
        }
    }
}

/**
 * \brief Find \p needle in \p haystack.
 *
 * Using the Knuth-Morris-Pratt string searching algorithm, attempt to locate
 * the string \p needle in the string \p haystack. In this algorithm, a table of
 * length \p nlen is computed to help keep track of partial matches. At position
 * \c p in this table, a number is stored that indicates how far back in \p
 * needle we must backtrack if the character at position \c p in \p needle does
 * not match the next character in \p haystack.
 *
 * Using this table, we can scan through \p haystack only once without having to
 * worry about backtracking in the case of a partial match.
 *
 * There are two 'nonsense' cases that can be passed in: when the \p needle is
 * the empty string, and when the \p haystack is an empty string. In both cases,
 * \c 0 is returned.
 *
 * \param [in] needle The string to search for in \p haystack.
 * \param [in] table A chunk of memory to use for computing the backtrack table.
 * This array must be at least \p nlen bytes long.
 * \param [in] nlen The number of characters in \p needle.
 * \param [in] haystack The string to search through for \p needle.
 * \param [in] hlen The number of characters in \p haystack.
 *
 * \return If \p needle is found in \p haystack, returns the position of the
 * match within \p haystack. If \p needle is not in \p haystack, returns \p
 * hlen. If either \p needle or \p haystack is an empty string, returns \c 0.
 *
 * \note This function runs in O(hlen + nlen) time and uses O(1) additional
 * space. The only O(nlen) temporary storage for this function is in \p table;
 * this is provided by the user to avoid any dynamic memory allocation in this
 * function.
 */
size_t kmp(const char *needle, size_t *table, size_t nlen,
        const char *haystack, size_t hlen)
{
    size_t match;
    size_t cur;

    if (nlen == 0)
        return 0;

    /* Computing the table runs in O(nlen) time. */
    _compute_table(needle, table, nlen);

    match = 0;
    cur = 0;

    /* Loops through at most hlen times. */
    while (match + cur < hlen)
    {
        /* If the current character matches... */
        if (needle[cur] == haystack[match + cur])
        {
            /* We have successfully found the needle in the haystack; cleanup
             * memory and return the offset of the match.
             */
            if (cur == nlen - 1)
            {
                return match;
            }
            /* Continue building the current match. */
            else
            {
                cur++;
            }
        }
        /* If the current character does not match... */
        else
        {
            /* Backtrack to the point of the previous partial match. */
            if (table[cur] != (size_t)~0)
            {
                match = match + cur - table[cur];
                cur = table[cur];
            }
            /* No partial match, backtrack to the beginning of needle. */
            else
            {
                match++;
                cur = 0;
            }
        }
    }

    /* If we reach here, that means that haystack does not contain needle. */
    return hlen;
}


#include "ghcommon.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double v;
    double f;
    double mm;
    double f64;
    double nf;
    double df;

    int n;

    int i;

    fraction fract;
    char *s = NULL;
    size_t count;

    while (TRUE)
    {
        size_t count = get_string(&s, "\n\nEnter <E>nglish or <M>etric: ");

        if (count == 0)
        {
            free_malloc(s);
            return SUCCESS;
        }

        v = get_double("\nEnter a decimal number or 0 to quit: ");

        if (s[0] == 'M' || s[0] == 'm')
            v = v * 0.03937008;

        free_malloc(s);

        if (v == 0.0)
            return SUCCESS;

        f = v - (int)v;
        i = (int)(v - f);
        mm = v / 0.03937008;

        fract = decimal_to_fraction(f, 1e-6);

        printf("\nInches: %3.4f", v);
        printf("\nMM: %3.2f", mm);

        if (v >= 1.0)
            printf("\nFraction: %d %d/%d", i, fract.n, fract.d);
        else
            printf("\nFraction: %d/%d", fract.n, fract.d);

        f64 = (f / 0.015625);

        n = (int)(f64 < 0 ? (f64 - 0.5) : (f64 + 0.5));
        nf = (double) n / 64.0;


        fract = decimal_to_fraction( nf, 1e-6);

        printf ("\nNearest standard fraction: ");

        if (nf >= 1.0)
            printf("%d %d/%d", i, fract.n, fract.d);
        else
            printf("%d/%d", fract.n, fract.d);

        printf ("   Diff = %3.4f\n\n", (double)n / 64.0 - v);
    }

    return SUCCESS;
}
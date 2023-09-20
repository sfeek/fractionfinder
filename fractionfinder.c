#include "ghcommon.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_fraction(double v)
{
    fraction fract;
    double i, f;

    f = modf(fabs(v), &i);

    if (v < 0)
        i = -i;

    fract = decimal_to_fraction(f, 1e-6);

    if (fract.n == 0)
    {
        printf("%d", (int)i);
        return;
    }

    if (i <= -1.0 || i >= 1.0)
        printf("%d %d/%d", (int)i, fract.n, fract.d);
    else
        printf("%d/%d", fract.n, fract.d);
}

void lookup_drill_size(double v)
{    
    char line[20];
    char sz[20] = "";
    char last_sz[20] = "";
    double value;
    int flag = 1;

    FILE *file = fopen("drillsize.csv", "r");

    if (file != NULL)
    {
        while (fgets(line, 20, file) != NULL)
        {
            sscanf(line, "%lf,%s", &value, sz);
            if (value >= (v - v * 0.05) && value <= (v + v * 0.05))
            {
                printf("  %3.4f : %s (%3.4f)\n", value, sz, value - v);
                flag = 0;
            }
        }
    }
    
    if (file)
        fclose(file);

    if (flag)
        printf("\n  Drill Size Not Found!");

    return;
}

int print_surrounding_fractions(double v, int dnom)
{
    double i, f, nf, i1, f1;

    if (dnom == 0)
        return FAIL_NUMBER;

    f = modf(v, &i);
    nf = f / (1 / (double)dnom);
    f1 = modf(nf, &i1);

    if (float_compare(f, i1 / (double)dnom, 1e-6)) return SUCCESS;

    printf("\nNearest 1/%d Fractions: ", dnom);
    print_fraction(i + i1 / (double)dnom);
    printf(" (%3.4f)", i1 / (double)dnom - f);
    printf("  &  ");
    print_fraction(i + (i1 + 1.0) / (double)dnom);
    printf(" (%3.4f)", (i1 + 1.0) / (double)dnom - f);
    
    return SUCCESS;
}

int main(void)
{
    double v;
    double f;
    double mm;
    double f64;
    double nf;
    double df;

    int metric_flag, n ,i;

    fraction fract;
    char *s = NULL;
    size_t count;

    while (TRUE)
    {
        while (TRUE)
        {
            count = get_string(&s, "\n\n<E>nglish, <M>etric or <Q>uit: ");

            if (count == 0)
            {
                free_malloc(s);
                continue;
            }

            if (s[0] == 'Q' || s[0] == 'q')
            {
                free_malloc(s);
                return SUCCESS;
            }

            if (s[0] == 'E' || s[0] == 'e')
            {
                free_malloc(s);
                metric_flag = FALSE;
                break;
            }

            if (s[0] == 'M' || s[0] == 'm')
            {
                free_malloc(s);
                metric_flag = TRUE;
                break;
            }

            free_malloc(s);
        }

        v = get_double("\nEnter a decimal number or 0 to quit: ");

        v = fabs(v);

        if (v == 0.0)
            return SUCCESS;

        if (metric_flag)
            v = v * 0.03937008;

        mm = v / 0.03937008;

        printf("\nInches: %3.4f", v);
        printf("\nMM: %3.2f", mm);
        printf("\n\nFraction: ");
        print_fraction(v);
        printf("\n");

        print_surrounding_fractions(v, 100);
        print_surrounding_fractions(v, 64);
        print_surrounding_fractions(v, 32);
        print_surrounding_fractions(v, 16);
        print_surrounding_fractions(v, 10);

        printf("\n\nStandard Drill Sizes +/- 5%%:\n");
        lookup_drill_size(v);

        printf("\n\n");
    }

    return SUCCESS;
}
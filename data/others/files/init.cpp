#include "scater.h"
#include "R_ext/Rdynload.h"
#include "R_ext/Visibility.h"

#define REGISTER(x, i) {#x, (DL_FUNC) &x, i}

extern "C" {

static const R_CallMethodDef all_call_entries[] = {
    REGISTER(norm_exprs, 6),
    REGISTER(ave_exprs, 4),

    REGISTER(combined_qc, 7),
    REGISTER(top_cumprop, 2),

    REGISTER(sum_row_counts, 5),
    REGISTER(sum_col_counts, 4),

    REGISTER(row_above, 4),
    REGISTER(col_above, 4),
    {NULL, NULL, 0}
};

void attribute_visible R_init_scater(DllInfo *dll) {
    R_registerRoutines(dll, NULL, all_call_entries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

}

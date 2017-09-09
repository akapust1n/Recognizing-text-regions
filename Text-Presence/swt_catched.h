#ifndef SWT_CATCHED_H
#define SWT_CATCHED_H
#include <stdio.h>
#include <cstdint>

typedef struct {
        int interval; /**< Intervals for scale invariant option. */
        int min_neighbors; /**< Minimal neighbors to make a detection valid, this is for scale-invariant version. */
        int scale_invariant; /**< Enable scale invariant swt (to scale to different sizes and then combine the results) */
        int direction; /**< SWT direction. (black to white or white to black). */
        double same_word_thresh[2]; /**< Overlapping more than 0.1 of the bigger one (0), and 0.9 of the smaller one (1) */
        /**
         * @name Canny parameters
         * @{
         */
        int size; /**< Parameters for [Canny edge detector](/lib/ccv-classic). */
        int low_thresh; /**< Parameters for [Canny edge detector](/lib/ccv-classic). */
        int high_thresh; /**< Parameters for [Canny edge detector](/lib/ccv-classic). */
        /** @} */
        /**
         * @name Geometry filtering parameters
         * @{
         */
        int max_height; /**< The maximum height for a letter. */
        int min_height; /**< The minimum height for a letter. */
        int min_area; /**< The minimum occupied area for a letter. */
        int letter_occlude_thresh;
        double aspect_ratio; /**< The maximum aspect ratio for a letter. */
        double std_ratio; /**< The inner-class standard derivation when grouping letters. */
        /** @} */
        /**
         * @name Grouping parameters
         * @{
         */
        double thickness_ratio; /**< The allowable thickness variance when grouping letters. */
        double height_ratio; /**< The allowable height variance when grouping letters. */
        int intensity_thresh; /**< The allowable intensity variance when grouping letters. */
        double distance_ratio; /**< The allowable distance variance when grouping letters. */
        double intersect_ratio; /**< The allowable intersect variance when grouping letters. */
        double elongate_ratio; /**< The allowable elongate variance when grouping letters. */
        int letter_thresh; /**< The allowable letter threshold. */
        /** @} */
        /**
         * @name Break textline into words
         * @{
         */
        int breakdown; /**< If breakdown text line into words. */
        double breakdown_ratio; /**< Apply [OSTU](/lib/ccv-classic) and if inter-class variance above the threshold, it will be break down into words. */
        /** @} */
} ccv_swt_param_t;

#define CCV_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))
#define ccv_min(a, b) (((a) < (b)) ? (a) : (b))

#define CCV_IMPLEMENT_MEDIAN(func_name, T) \
T func_name(T* buf, int low, int high) \
{                                                    \
    T w;                                             \
    int middle, ll, hh;                              \
    int median = (low + high) / 2;                   \
    for (;;)                                         \
    {                                                \
        if (high <= low)                             \
            return buf[median];                      \
        if (high == low + 1)                         \
        {                                            \
            if (buf[low] > buf[high])                \
                CCV_SWAP(buf[low], buf[high], w);    \
            return buf[median];                      \
        }                                            \
        middle = (low + high) / 2;                   \
        if (buf[middle] > buf[high])                 \
            CCV_SWAP(buf[middle], buf[high], w);     \
        if (buf[low] > buf[high])                    \
            CCV_SWAP(buf[low], buf[high], w);        \
        if (buf[middle] > buf[low])                  \
            CCV_SWAP(buf[middle], buf[low], w);      \
        CCV_SWAP(buf[middle], buf[low + 1], w);      \
        ll = low + 1;                                \
        hh = high;                                   \
        for (;;)                                     \
        {                                            \
            do ll++; while (buf[low] > buf[ll]);     \
            do hh--; while (buf[hh] > buf[low]);     \
            if (hh < ll)                             \
                break;                               \
            CCV_SWAP(buf[ll], buf[hh], w);           \
        }                                            \
        CCV_SWAP(buf[low], buf[hh], w);              \
        if (hh <= median)                            \
            low = ll;                                \
        else if (hh >= median)                       \
            high = hh - 1;                           \
    }                                                \
}

#define CCV_IMPLEMENT_QSORT_EX(func_name, T, LT, swap_func, user_data_type)                     \
void func_name(T *array, size_t total, user_data_type aux)                                      \
{                                                                                               \
    int isort_thresh = 7;                                                                       \
    T t;                                                                                        \
    int sp = 0;                                                                                 \
                                                                                                \
    struct                                                                                      \
    {                                                                                           \
        T *lb;                                                                                  \
        T *ub;                                                                                  \
    }                                                                                           \
    stack[48];                                                                                  \
                                                                                                \
    if( total <= 1 )                                                                            \
        return;                                                                                 \
                                                                                                \
    stack[0].lb = array;                                                                        \
    stack[0].ub = array + (total - 1);                                                          \
                                                                                                \
    while( sp >= 0 )                                                                            \
    {                                                                                           \
        T* left = stack[sp].lb;                                                                 \
        T* right = stack[sp--].ub;                                                              \
                                                                                                \
        for(;;)                                                                                 \
        {                                                                                       \
            int i, n = (int)(right - left) + 1, m;                                              \
            T* ptr;                                                                             \
            T* ptr2;                                                                            \
                                                                                                \
            if( n <= isort_thresh )                                                             \
            {                                                                                   \
            insert_sort:                                                                        \
                for( ptr = left + 1; ptr <= right; ptr++ )                                      \
                {                                                                               \
                    for( ptr2 = ptr; ptr2 > left && LT(ptr2[0],ptr2[-1], aux); ptr2--)          \
                        swap_func( ptr2[0], ptr2[-1], array, aux, t );                          \
                }                                                                               \
                break;                                                                          \
            }                                                                                   \
            else                                                                                \
            {                                                                                   \
                T* left0;                                                                       \
                T* left1;                                                                       \
                T* right0;                                                                      \
                T* right1;                                                                      \
                T* pivot;                                                                       \
                T* a;                                                                           \
                T* b;                                                                           \
                T* c;                                                                           \
                int swap_cnt = 0;                                                               \
                                                                                                \
                left0 = left;                                                                   \
                right0 = right;                                                                 \
                pivot = left + (n/2);                                                           \
                                                                                                \
                if( n > 40 )                                                                    \
                {                                                                               \
                    int d = n / 8;                                                              \
                    a = left, b = left + d, c = left + 2*d;                                     \
                    left = LT(*a, *b, aux) ? (LT(*b, *c, aux) ? b : (LT(*a, *c, aux) ? c : a))  \
                                      : (LT(*c, *b, aux) ? b : (LT(*a, *c, aux) ? a : c));      \
                                                                                                \
                    a = pivot - d, b = pivot, c = pivot + d;                                    \
                    pivot = LT(*a, *b, aux) ? (LT(*b, *c, aux) ? b : (LT(*a, *c, aux) ? c : a)) \
                                      : (LT(*c, *b, aux) ? b : (LT(*a, *c, aux) ? a : c));      \
                                                                                                \
                    a = right - 2*d, b = right - d, c = right;                                  \
                    right = LT(*a, *b, aux) ? (LT(*b, *c, aux) ? b : (LT(*a, *c, aux) ? c : a)) \
                                      : (LT(*c, *b, aux) ? b : (LT(*a, *c, aux) ? a : c));      \
                }                                                                               \
                                                                                                \
                a = left, b = pivot, c = right;                                                 \
                pivot = LT(*a, *b, aux) ? (LT(*b, *c, aux) ? b : (LT(*a, *c, aux) ? c : a))     \
                                   : (LT(*c, *b, aux) ? b : (LT(*a, *c, aux) ? a : c));         \
                if( pivot != left0 )                                                            \
                {                                                                               \
                    swap_func( *pivot, *left0, array, aux, t );                                 \
                    pivot = left0;                                                              \
                }                                                                               \
                left = left1 = left0 + 1;                                                       \
                right = right1 = right0;                                                        \
                                                                                                \
                for(;;)                                                                         \
                {                                                                               \
                    while( left <= right && !LT(*pivot, *left, aux) )                           \
                    {                                                                           \
                        if( !LT(*left, *pivot, aux) )                                           \
                        {                                                                       \
                            if( left > left1 )                                                  \
                                swap_func( *left1, *left, array, aux, t );                      \
                            swap_cnt = 1;                                                       \
                            left1++;                                                            \
                        }                                                                       \
                        left++;                                                                 \
                    }                                                                           \
                                                                                                \
                    while( left <= right && !LT(*right, *pivot, aux) )                          \
                    {                                                                           \
                        if( !LT(*pivot, *right, aux) )                                          \
                        {                                                                       \
                            if( right < right1 )                                                \
                                swap_func( *right1, *right, array, aux, t );                    \
                            swap_cnt = 1;                                                       \
                            right1--;                                                           \
                        }                                                                       \
                        right--;                                                                \
                    }                                                                           \
                                                                                                \
                    if( left > right )                                                          \
                        break;                                                                  \
                    swap_func( *left, *right, array, aux, t );                                  \
                    swap_cnt = 1;                                                               \
                    left++;                                                                     \
                    right--;                                                                    \
                }                                                                               \
                                                                                                \
                if( swap_cnt == 0 )                                                             \
                {                                                                               \
                    left = left0, right = right0;                                               \
                    goto insert_sort;                                                           \
                }                                                                               \
                                                                                                \
                n = ccv_min( (int)(left1 - left0), (int)(left - left1) );                       \
                for( i = 0; i < n; i++ )                                                        \
                    swap_func( left0[i], left[i-n], array, aux, t );                            \
                                                                                                \
                n = ccv_min( (int)(right0 - right1), (int)(right1 - right) );                   \
                for( i = 0; i < n; i++ )                                                        \
                    swap_func( left[i], right0[i-n+1], array, aux, t );                         \
                n = (int)(left - left1);                                                        \
                m = (int)(right1 - right);                                                      \
                if( n > 1 )                                                                     \
                {                                                                               \
                    if( m > 1 )                                                                 \
                    {                                                                           \
                        if( n > m )                                                             \
                        {                                                                       \
                            stack[++sp].lb = left0;                                             \
                            stack[sp].ub = left0 + n - 1;                                       \
                            left = right0 - m + 1, right = right0;                              \
                        }                                                                       \
                        else                                                                    \
                        {                                                                       \
                            stack[++sp].lb = right0 - m + 1;                                    \
                            stack[sp].ub = right0;                                              \
                            left = left0, right = left0 + n - 1;                                \
                        }                                                                       \
                    }                                                                           \
                    else                                                                        \
                        left = left0, right = left0 + n - 1;                                    \
                }                                                                               \
                else if( m > 1 )                                                                \
                    left = right0 - m + 1, right = right0;                                      \
                else                                                                            \
                    break;                                                                      \
            }                                                                                   \
        }                                                                                       \
    }                                                                                           \
}
#define _ccv_qsort_default_swap(a, b, array, aux, t) CCV_SWAP((a), (b), (t))

#define CCV_IMPLEMENT_QSORT(func_name, T, cmp) \
    CCV_IMPLEMENT_QSORT_EX(func_name, T, cmp, _ccv_qsort_default_swap, int)

typedef union {
    unsigned char* u8;
    int* i32;
    float* f32;
    int64_t* i64;
    double* f64;
} ccv_matrix_cell_t;

typedef struct {
    int type;
    uint64_t sig;
    int refcount;
    int rows;
    int cols;
    int step;
    union {
        unsigned char u8;
        int i32;
        float f32;
        int64_t i64;
        double f64;
        void* p;
    } tag;
    ccv_matrix_cell_t data;
} ccv_dense_matrix_t;

enum {
    CCV_C1 = 0x001,
    CCV_C2 = 0x002,
    CCV_C3 = 0x003,
    CCV_C4 = 0x004,
};
# define assert(expr)		(__ASSERT_VOID_CAST (0))
# define __ASSERT_VOID_CAST (void)

#define ccv_sign_with_format(size, string, ...) \
    char INTERNAL_CATCH_UNIQUE_NAME(_ccv_identifier_)[(size)]; \
    memset(INTERNAL_CATCH_UNIQUE_NAME(_ccv_identifier_), 0, (size)); \
    snprintf(INTERNAL_CATCH_UNIQUE_NAME(_ccv_identifier_), (size), (string), ##__VA_ARGS__); \
    size_t INTERNAL_CATCH_UNIQUE_NAME(_ccv_string_size_) = (size);

#ifndef INTERNAL_CATCH_UNIQUE_NAME_LINE2
#define INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line ) name##line
#endif
#ifndef INTERNAL_CATCH_UNIQUE_NAME_LINE
#define INTERNAL_CATCH_UNIQUE_NAME_LINE( name, line ) INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line )
#endif
#ifndef INTERNAL_CATCH_UNIQUE_NAME
#define INTERNAL_CATCH_UNIQUE_NAME( name ) INTERNAL_CATCH_UNIQUE_NAME_LINE( name, __LINE__ )
#endif

#define CCV_EOF_SIGN ((uint64_t)0)

enum {
    CCV_8U  = 0x01000,
    CCV_32S = 0x02000,
    CCV_32F = 0x04000,
    CCV_64S = 0x08000,
    CCV_64F = 0x10000,
};


#define CCV_GET_DATA_TYPE(x) ((x) & 0xFF000)
#define CCV_GET_DATA_TYPE_SIZE(x) _ccv_get_data_type_size[CCV_GET_DATA_TYPE(x) >> 12]
#define CCV_MAX_CHANNEL (0xFFF)
#define CCV_GET_CHANNEL(x) ((x) & 0xFFF)
#define CCV_ALL_DATA_TYPE (CCV_8U | CCV_32S | CCV_32F | CCV_64S | CCV_64F)

uint64_t ccv_cache_generate_signature(const char* msg, int len, uint64_t sig_start, ...)
{
    blk_SHA_CTX ctx;
    blk_SHA1_Init(&ctx);
    uint64_t sigi;
    va_list arguments;
    va_start(arguments, sig_start);
    for (sigi = sig_start; sigi != 0; sigi = va_arg(arguments, uint64_t))
        blk_SHA1_Update(&ctx, &sigi, 8);
    va_end(arguments);
    blk_SHA1_Update(&ctx, msg, len);
    union {
        uint64_t u;
        uint8_t chr[20];
    } sig;
    blk_SHA1_Final(sig.chr, &ctx);
    return sig.u;
}
typedef struct {
    unsigned long long size;
    unsigned int H[5];
    unsigned int W[16];
} blk_SHA_CTX;

ccv_dense_matrix_t* ccv_dense_matrix_new(int rows, int cols, int type, void* data, uint64_t sig)
{
    ccv_dense_matrix_t* mat;
    if (ccv_cache_opt && sig != 0 && !data && !(type & CCV_NO_DATA_ALLOC))
    {
        uint8_t type;
        mat = (ccv_dense_matrix_t*)ccv_cache_out(&ccv_cache, sig, &type);
        if (mat)
        {
            assert(type == 0);
            mat->type |= CCV_GARBAGE; // set the flag so the upper level function knows this is from recycle-bin
            mat->refcount = 1;
            return mat;
        }
    }
    if (type & CCV_NO_DATA_ALLOC)
    {
        mat = (ccv_dense_matrix_t*)ccmalloc(sizeof(ccv_dense_matrix_t));
        mat->type = (CCV_GET_CHANNEL(type) | CCV_GET_DATA_TYPE(type) | CCV_MATRIX_DENSE | CCV_NO_DATA_ALLOC) & ~CCV_GARBAGE;
        mat->data.u8 = data;
    } else {
        mat = (ccv_dense_matrix_t*)(data ? data : ccmalloc(ccv_compute_dense_matrix_size(rows, cols, type)));
        mat->type = (CCV_GET_CHANNEL(type) | CCV_GET_DATA_TYPE(type) | CCV_MATRIX_DENSE) & ~CCV_GARBAGE;
        mat->type |= data ? CCV_UNMANAGED : CCV_REUSABLE; // it still could be reusable because the signature could be derived one.
        mat->data.u8 = (unsigned char*)(mat + 1);
    }
    mat->sig = sig;
    mat->rows = rows;
    mat->cols = cols;
    mat->step = (cols * CCV_GET_DATA_TYPE_SIZE(type) * CCV_GET_CHANNEL(type) + 3) & -4;
    mat->refcount = 1;
    return mat;
}
ccv_dense_matrix_t* ccv_dense_matrix_renew(ccv_dense_matrix_t* x, int rows, int cols, int types, int prefer_type, uint64_t sig)
{
    if (x != 0)
    {
        assert(x->rows == rows && x->cols == cols && (CCV_GET_DATA_TYPE(x->type) & types) && (CCV_GET_CHANNEL(x->type) == CCV_GET_CHANNEL(types)));
        prefer_type = CCV_GET_DATA_TYPE(x->type) | CCV_GET_CHANNEL(x->type);
    }
    if (sig != 0)
        sig = ccv_cache_generate_signature((const char*)&prefer_type, sizeof(int), sig, CCV_EOF_SIGN);
    if (x == 0)
    {
        x = ccv_dense_matrix_new(rows, cols, prefer_type, 0, sig);
    } else {
        x->sig = sig;
    }
    return x;
}
#endif // SWT_CATCHED_H

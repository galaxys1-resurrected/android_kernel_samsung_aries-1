extern int dynbigmem_control;
extern struct s5p_media_device* aries_get_media_devs (int* nr_devs);


enum mem_modes {
    normal = 0,
    nocam = 1,
    bigmem = 2,
    xlmem = 3,
};

struct mdev_size {
    size_t fimc0;
    size_t fimc1;
    size_t mfc0;
    size_t mfc1;    
};

struct mdev_size media_dev_sizes[] = {
    {
        .fimc0 = 12288 * SZ_1K, 
        .fimc1 = 12288 * SZ_1K, 
        .mfc0 = 14336 * SZ_1K,
        .mfc1 = 21504 * SZ_1K,
    },
    {
        .fimc0 = 0,
        .fimc1 = 0,
        .mfc0 = 0,
        .mfc1 = 0,
    },
    {
        .fimc0 = 5000 * SZ_1K,
        .fimc1 = 5000 * SZ_1K,
        .mfc0 = 11264 * SZ_1K,
        .mfc1 = 11264 * SZ_1K,
    },
    {
        .fimc0 = 12288 * SZ_1K,
        .fimc1 = 12288 * SZ_1K,
        .mfc0 = 11264 * SZ_1K,
        .mfc1 = 11264 * SZ_1K,
    },
};

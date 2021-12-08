#ifndef COMMUNICATOR_GLOBAL_H
#define COMMUNICATOR_GLOBAL_H

/** product GUID */
#define PRODUCT_GUID  "74F2C5FD-2F95-46be-B67C-FFA200D69012"

#define GET_ARRAY_LEN(array,len) {len = (sizeof(array) / sizeof(array[0]));}

#define AVAILED_GUID(x, y)  { \
    do { \
        const char  *PLATFORM_GUID[] = { \
            "EB8AFFA5-0710-47e6-8F53-55CAE55E1915", \
            "50BAC747-7D02-4969-AF79-45EE47365C81", \
            "D49170C0-B076-4795-B079-0F97560485AF" \
        }; \
        int len; \
        GET_ARRAY_LEN(PLATFORM_GUID, len); \
        for (int i=0; i<len; i++) { \
            if (strcmp(PLATFORM_GUID[i], (x)) == 0) { \
                y = true; \
            } \
        } \
        y = false; \
    }while(0); \
}

#define SECURITY_FREE(x) { \
    if (x) { \
        delete (x); \
        (x) = NULL; \
    }\
}

#define ASSERT_ZERO(x, y) if(!x) return y;
#define SECURITY_CHECK(x, y, z) if(x != y) return z;

#define BASIC_API_URL	"Api/Api.php"

#define TESPAN_DEFAULT_VALUE    300

// error code
// 1, header has no I and V info
// 2, header I value empty
// 3, header no eid info
// 4, header no guid
// 5, http format error
// 6, header info error
enum CommunityError {
    API_I_HEAD_ERROR    = 1,
    API_I_HEADNULL_ERROR,
    API_I_EID_ERROR,
    API_I_GUID_ERROR,
    API_BODY_ERROR,
    API_I_HEADER_ERROR
};



#endif // COMMUNICATOR_GLOBAL_H

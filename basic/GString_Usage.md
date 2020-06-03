# GString 使用

GString是一种动态字符串数据结构，提供了丰富灵活方便的API，无需手工分配内存，无需担心缓冲区溢出。

    #include <glib.h>
    #include <stdio.h>

    // gcc -std=c99 gstring-demo.c -o gstring-demo `pkg-config --cflags --libs glib-2.0`
    int main(void) {
        printf("GString demo\n");
        GString *string1 = g_string_new("123456");
        printf("string1 length: %lu\n", string1->len); // 6
        g_string_free(string1, TRUE);

        char * char_source = g_strdup("123456");
        GString *string2 = g_string_new(char_source);
        g_free(char_source);
        char * ret2 = g_string_free(string2, FALSE);
        printf("Original char source: %s\n", ret2);
        g_free(ret2);

        char * char_source3 = g_strdup("123456");
        GString *string3 = g_string_new(char_source3);
        char * ret3 = g_string_free(string3, TRUE); // char_source is not freed.
        printf("Original char source: %s\n", ret3); // NULL

        return 0;
    }

对于 g_string_free 方法，需要注意的是它的第二个参数，当为TRUE时，它返回NULL；当为FALSE时，它返回一个gchar *指针，用户可以使用它，并使用 g_free 来释放
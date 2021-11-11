#include <errno.h>
#include "librim.c"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define img2rim 0
#define rim2img 1

#ifndef CONVERSION
    #define CONVERSION img2rim
#endif

typedef enum {
    EXT_UNKNOWN,
    EXT_PNG,
    EXT_JPG,
    EXT_BMP,
    EXT_TGA,
    EXT_HDR
} extension;

bool _streq(char* s1, char* s2){return strcmp(s1, s2) == 0;}

extension get_fext(char* fn)
{
    char* afterdot = strrchr(fn, '.');
    #define _adeq(o) _streq(afterdot, (o))
    if (afterdot != NULL)
    {
        afterdot++;
        if (_adeq("png") || _adeq("PNG")){return EXT_PNG;}
        if (_adeq("jpg") || _adeq("jpeg") || _adeq("JPG") || _adeq("JPEG")){return EXT_JPG;}
        if (_adeq("bmp") || _adeq("BMP")){return EXT_BMP;}
        if (_adeq("tga") || _adeq("TGA") || _adeq("targa") || _adeq("TARGA")){return EXT_TGA;}
        if (_adeq("hdr") || _adeq("HDR")){return EXT_HDR;}
    }
    return EXT_UNKNOWN;
    #undef _adeq
}

int main(int argc, char** argv)
{
    if (argc < 3){puts("ERROR: I need 2 arguments ( <input-file> <output-file> )"); return -1;}
    char *infile = argv[1], *outfile = argv[2];
    #if (CONVERSION == img2rim)
        int w, h, nc;
        unsigned char* inpix = stbi_load(infile, &w, &h, &nc, 0);
        if (inpix == NULL){printf("ERROR: Failed to open input file %s\nreason: %s\n", infile, strerror(errno)); return -2;}
        rim_t* rim = blank_rim(w, h, nc, false);
        memcpy(rim->pixels, inpix, w*h*nc);
        FILE* outf = fopen(outfile, "wb");
        if (outf == NULL){printf("ERROR: Failed to open output file %s\nreason: %s\n", outfile, strerror(errno)); return -3;}
        save_rimf_nef(rim, outf);
        fclose(outf);
        free_rim(&rim);
    #elif (CONVERSION == rim2img)
        #define getarg(_i) ((_i) >= argc ? NULL : argv[(_i)])
        char* cargstr = NULL;
        char cname[65] = { };
        int jpg_quality = 25;
        int inxt;
        for (int i = 3; i < argc; i++)
        {
            cargstr = getarg(i);
            if (*cargstr == '-')
            {
                if (strchr(cargstr, '\0')-(cargstr+1) > 64){puts("ERROR: argument name overflow!"); return -4;}
                memset(cname, 0, 65);
                strcpy(cname, cargstr+1);
                
                if (_streq(cname, "jpg_quality"))
                {
                    inxt = ++i;
                    cargstr = getarg(inxt);
                    if (cargstr == NULL){break;}
                    sscanf(cargstr, "%i", &jpg_quality);
                }
                else if (_streq(cname, "tga_rle"))
                {
                    inxt = ++i;
                    cargstr = getarg(inxt);
                    if (cargstr == NULL){break;}
                    sscanf(cargstr, "%i", &stbi_write_tga_with_rle);
                }
                else if (_streq(cname, "png_complevel"))
                {
                    inxt = ++i;
                    cargstr = getarg(inxt);
                    if (cargstr == NULL){break;}
                    sscanf(cargstr, "%i", &stbi_write_png_compression_level);
                }
            }
        }
        rim_t* rim = NULL;
        FILE* fp = fopen(infile, "rb");
        if (fp == NULL){printf("ERROR: Failed to open input file %s\nreason: %s\n", infile, strerror(errno)); return -2;}
        rim_status rst = load_rimf_nef(fp, &rim);
        if (rst != RIMSTAT_OK)
        {
            printf("ERROR: Failed to load data from %s\nreason: %s\n", infile, rimstatus2str(rst));
            return -5;
        }
        fclose(fp);
        extension ext = get_fext(outfile);
        int w = (int)rim->width, h = (int)rim->height, comp = (int)rim->colorfmt;
        switch (ext)
        {
            case EXT_PNG:
                stbi_write_png(outfile, w, h, comp, rim->pixels, w*comp);
                break;
            case EXT_JPG:
                stbi_write_jpg(outfile, w, h, comp, rim->pixels, jpg_quality);
                break;
            case EXT_TGA:
                stbi_write_tga(outfile, w, h, comp, rim->pixels);
                break;
            default:
                puts("ERROR: unknown or unsupported output file format");
                return -6;
        }
        free_rim(&rim);
    #else
        #error "Unknown CONVERSION value: " (#CONVERSION)
    #endif
    return 0;
}

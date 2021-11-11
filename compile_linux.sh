i2r_build="$compiler rimcvt.c -o img2rim -lm -D CONVERSION=img2rim"
r2i_build="$compiler rimcvt.c -o rim2img -lm -D CONVERSION=rim2img"
echo Building img2rim...
echo "> $i2r_build"
$i2r_build
echo Building rim2img...
echo "> $r2i_build"
$r2i_build
echo Done!
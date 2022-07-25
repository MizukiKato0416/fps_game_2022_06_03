xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 105;
 -453.28301;255.75015;578.80875;,
 -453.28301;255.75015;-586.09557;,
 -453.28301;-121.61355;-586.09557;,
 -453.28301;-121.61355;578.80875;,
 -75.91931;255.75015;-586.09557;,
 -75.91931;-121.61355;-586.09557;,
 -75.91931;255.75015;578.80875;,
 -75.91931;-121.61355;578.80875;,
 -486.97100;240.99839;195.93712;,
 -486.97100;240.99839;-581.80865;,
 -486.97100;222.99060;-581.80865;,
 -486.97100;222.99060;195.93712;,
 -275.76626;465.02525;-581.80865;,
 -275.76626;447.01739;-581.80865;,
 -246.59317;465.02525;-581.80865;,
 -246.59317;447.01739;-581.80865;,
 -35.38855;240.99839;-581.80865;,
 -35.38855;222.99060;-581.80865;,
 -35.38855;240.99839;195.93712;,
 -35.38855;222.99060;195.93712;,
 -246.59317;465.02525;195.93712;,
 -246.59317;447.01739;195.93712;,
 -275.76609;465.02525;195.93712;,
 -275.76609;447.01739;195.93712;,
 -447.55152;514.75304;200.97458;,
 -77.72621;514.75304;200.97458;,
 -77.72621;162.83969;200.97458;,
 -447.55152;162.83969;200.97458;,
 -77.72621;514.75304;578.30885;,
 -77.72621;162.83969;578.30885;,
 -447.55152;514.75304;578.30885;,
 -447.55152;162.83969;578.30885;,
 -64.67380;514.51085;597.48853;,
 -463.31783;514.51085;597.48853;,
 -463.31783;496.50299;597.48853;,
 -64.67380;496.50299;597.48853;,
 -463.31783;738.53761;404.41900;,
 -463.31783;720.52980;404.41900;,
 -463.31783;738.53761;377.75079;,
 -463.31783;720.52980;377.75079;,
 -463.31783;514.51085;184.68126;,
 -463.31783;496.50299;184.68126;,
 -64.67380;514.51085;184.68126;,
 -64.67380;496.50299;184.68126;,
 -64.67380;738.53761;377.75079;,
 -64.67380;720.52980;377.75079;,
 -64.67380;738.53761;404.41880;,
 -64.67380;720.52980;404.41880;,
 -62.10251;252.38882;-585.11153;,
 -465.09083;251.89141;-583.57467;,
 -261.10374;466.23258;-583.15245;,
 -83.38960;513.39239;590.00562;,
 -84.92649;512.89512;192.44032;,
 -64.60995;736.36515;390.03453;,
 -438.64014;513.39239;207.04702;,
 -437.10308;512.89512;575.39195;,
 -465.80743;733.97945;391.30333;,
 -363.24336;739.35353;495.45450;,
 -303.33565;739.35353;495.45450;,
 -303.33565;572.81114;495.45450;,
 -363.24336;572.81114;495.45450;,
 -303.33565;739.35353;537.49752;,
 -303.33565;572.81114;537.49752;,
 -363.24336;739.35353;537.49752;,
 -363.24336;572.81114;537.49752;,
 -456.33956;-102.59505;-595.89605;,
 -71.70475;-102.63648;-595.89605;,
 -71.70475;-140.60398;-595.89605;,
 -456.33956;-144.36018;-595.89605;,
 -71.70475;-102.63648;580.68560;,
 -71.70475;-140.60398;580.68560;,
 -456.33956;-102.59505;580.68560;,
 -456.33956;-144.36018;580.68560;,
 -454.56402;292.74380;208.37993;,
 -82.80476;292.74380;208.37993;,
 -82.80476;249.20703;208.37993;,
 -454.56402;249.20703;208.37993;,
 -74.50991;292.74380;582.82750;,
 -74.50991;249.20703;582.82750;,
 -454.56402;292.74380;585.74283;,
 -454.56402;249.20703;585.74283;,
 -71.43865;161.56260;-60.80349;,
 -71.43865;161.56260;109.09506;,
 -71.43865;-8.33596;109.09506;,
 -71.43865;-8.33596;-60.80349;,
 -71.43865;161.56260;-460.49742;,
 -71.43865;161.56260;-167.88699;,
 -71.43865;-8.33596;-167.88699;,
 -71.43865;-8.33596;-460.49742;,
 -450.63072;463.27379;468.85030;,
 -450.63072;463.27379;295.52769;,
 -450.63072;342.84463;295.52769;,
 -450.63072;342.84463;468.85030;,
 -453.78380;161.56260;203.78092;,
 -453.78380;161.56260;-88.82964;,
 -453.78380;-8.33596;-88.82964;,
 -453.78380;-8.33596;203.78092;,
 -453.78380;161.56260;548.28848;,
 -453.78380;161.56260;255.67808;,
 -453.78380;-8.33596;255.67808;,
 -453.78380;-8.33596;548.28848;,
 -453.78380;161.56260;-167.88699;,
 -453.78380;161.56260;-460.49742;,
 -453.78380;-8.33596;-460.49742;,
 -453.78380;-8.33596;-167.88699;;
 
 56;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,0,3,7;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,8,11,23;,
 4;18,16,14,20;,
 4;20,14,12,22;,
 4;22,12,9,8;,
 4;21,15,17,19;,
 4;11,10,13,23;,
 4;24,25,26,27;,
 4;25,28,29,26;,
 4;28,30,31,29;,
 4;30,24,27,31;,
 4;32,33,34,35;,
 4;33,36,37,34;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,32,35,47;,
 4;42,40,38,44;,
 4;44,38,36,46;,
 4;46,36,33,32;,
 4;45,39,41,43;,
 4;35,34,37,47;,
 3;48,49,50;,
 3;51,52,53;,
 3;54,55,56;,
 4;57,58,59,60;,
 4;58,61,62,59;,
 4;61,63,64,62;,
 4;63,57,60,64;,
 4;63,61,58,57;,
 4;65,66,67,68;,
 4;66,69,70,67;,
 4;69,71,72,70;,
 4;71,65,68,72;,
 4;73,74,75,76;,
 4;74,77,78,75;,
 4;77,79,80,78;,
 4;79,73,76,80;,
 4;81,82,83,84;,
 4;85,86,87,88;,
 4;89,90,91,92;,
 4;93,94,95,96;,
 4;97,98,99,100;,
 4;101,102,103,104;;
 
 MeshMaterialList {
  4;
  56;
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MESH\\rennga_white.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MESH\\darkoak.jpeg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MESH\\window.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MESH\\square.png";
   }
  }
 }
 MeshNormals {
  50;
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.929415;0.369037;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.929415;0.369037;0.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  0.396247;0.918144;0.000000;,
  -0.396246;0.918144;0.000000;,
  -0.727623;-0.685977;0.000000;,
  0.727623;-0.685978;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.348206;0.937418;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.348206;-0.937418;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.909073;-0.416636;,
  0.000000;0.909073;0.416636;,
  0.000000;-0.652829;0.757505;,
  -0.000000;-0.652829;-0.757505;,
  -0.003821;0.005606;-0.999977;,
  0.996179;-0.087258;-0.003742;,
  -0.992086;-0.125501;0.003970;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.999755;0.000000;-0.022147;,
  0.007671;0.000000;0.999971;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;;
  56;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,5,5;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;7,7,9,9;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;7,7,10,10;,
  4;10,10,11,11;,
  4;11,11,4,4;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,19,19;,
  4;20,20,20,20;,
  4;20,20,20,20;,
  4;20,20,20,20;,
  4;21,21,23,23;,
  4;22,22,22,22;,
  4;22,22,22,22;,
  4;22,22,22,22;,
  4;21,21,24,24;,
  4;24,24,25,25;,
  4;25,25,18,18;,
  4;26,26,26,26;,
  4;27,27,27,27;,
  3;28,28,28;,
  3;29,29,29;,
  3;30,30,30;,
  4;31,31,31,31;,
  4;32,32,32,32;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;39,39,39,39;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;42,42,42,42;,
  4;43,43,43,43;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;48,48,48,48;,
  4;49,49,49,49;;
 }
 MeshTextureCoords {
  105;
  -0.826001;-0.286974;,
  2.531548;-0.286974;,
  2.531548;0.874220;,
  -0.826001;0.874220;,
  2.124889;-0.286974;,
  2.124889;0.874220;,
  -1.232659;-0.286974;,
  -1.232659;0.874220;,
  -3.113391;-7.115638;,
  -23.367985;-7.115638;,
  -23.367985;-6.546585;,
  -3.113391;-6.546585;,
  -19.587706;-14.194969;,
  -19.587706;-13.625915;,
  -19.065548;-14.194969;,
  -19.065548;-13.625915;,
  -15.285271;-7.115638;,
  -15.285271;-6.546585;,
  4.969323;-7.115638;,
  4.969323;-6.546585;,
  1.189046;-14.194969;,
  1.189046;-13.625915;,
  0.666890;-14.194969;,
  0.666890;-13.625915;,
  0.256836;-1.083957;,
  -0.141699;-1.083957;,
  -0.141699;-0.001077;,
  0.256836;-0.001077;,
  -1.229272;-1.083957;,
  -1.229272;-0.001077;,
  -0.830737;-1.083957;,
  -0.830737;-0.001077;,
  14.902637;-15.758732;,
  7.767448;-15.758732;,
  7.767448;-15.189677;,
  14.902637;-15.189677;,
  2.739398;-22.838060;,
  2.739398;-22.269007;,
  2.044885;-22.838060;,
  2.044885;-22.269007;,
  -2.983165;-15.758732;,
  -2.983165;-15.189677;,
  4.152024;-15.758732;,
  4.152024;-15.189677;,
  9.180074;-22.838060;,
  9.180074;-22.269007;,
  9.874580;-22.838060;,
  9.874580;-22.269007;,
  -15.849430;-7.475580;,
  -23.022350;-7.459861;,
  -19.360262;-14.233121;,
  14.372773;-15.723389;,
  3.991595;-15.707674;,
  9.501119;-22.769409;,
  -1.959003;-15.723389;,
  7.661202;-15.707674;,
  2.353270;-22.694021;,
  -0.682782;-1.775079;,
  -0.747341;-1.775079;,
  -0.747341;-1.262609;,
  -0.682782;-1.262609;,
  -0.868519;-1.775079;,
  -0.868519;-1.262609;,
  -0.803961;-1.775079;,
  -0.803961;-1.262609;,
  -8.509949;1.731976;,
  -5.795115;1.732474;,
  -5.795115;2.188393;,
  -8.509949;2.233498;,
  5.635134;1.732474;,
  5.635134;2.188393;,
  2.920300;1.731976;,
  2.920300;2.233498;,
  -0.684040;-3.015310;,
  1.939915;-3.015310;,
  1.939915;-2.492514;,
  -0.684040;-2.492514;,
  5.636143;-3.015310;,
  5.636143;-2.492514;,
  2.981962;-3.015310;,
  2.981962;-2.492514;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;;
 }
}

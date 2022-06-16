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
 154;
 61.89544;29.29231;-48.03854;,
 138.28230;36.74631;-55.49277;,
 140.42447;8.30172;-61.34570;,
 61.89544;8.30172;-55.45970;,
 207.96751;29.29231;-48.03854;,
 212.25224;8.30172;-52.32302;,
 138.28230;-20.14310;-55.49277;,
 61.89544;-12.68887;-48.03854;,
 207.96751;-12.68887;-48.03854;,
 217.77325;39.09805;0.00000;,
 222.05791;8.30172;0.00000;,
 207.96751;29.29231;48.03854;,
 212.25224;8.30172;52.32302;,
 217.77325;-22.49459;-0.00000;,
 207.96751;-12.68887;48.03854;,
 138.28230;36.74631;55.49277;,
 140.42447;8.30172;61.34570;,
 61.89544;29.29231;48.03854;,
 61.89544;8.30172;55.45970;,
 138.28230;-20.14310;55.49277;,
 61.89544;-12.68887;48.03854;,
 143.18515;50.14138;0.00000;,
 61.89544;-12.68887;-48.03854;,
 138.28230;-20.14310;-55.49277;,
 143.18515;-33.53796;-0.00000;,
 61.89544;-29.67287;-0.00000;,
 207.96751;-12.68887;-48.03854;,
 217.77325;-22.49459;-0.00000;,
 207.96751;-12.68887;48.03854;,
 -59.34723;29.29231;48.03854;,
 -135.73409;36.74631;55.49277;,
 -137.87643;8.30172;61.34570;,
 -59.34723;8.30172;55.45970;,
 -205.41972;29.29231;48.03854;,
 -209.70441;8.30172;52.32321;,
 -135.73409;-20.14310;55.49277;,
 -59.34723;-12.68887;48.03854;,
 -205.41972;-12.68887;48.03854;,
 -215.22545;39.09805;0.00000;,
 -219.51012;8.30172;0.00000;,
 -205.41972;29.29231;-48.03854;,
 -209.70441;8.30172;-52.32302;,
 -215.22545;-22.49459;0.00000;,
 -205.41972;-12.68887;-48.03854;,
 -135.73409;36.74631;-55.49277;,
 -137.87643;8.30172;-61.34570;,
 -209.70441;8.30172;-52.32302;,
 -59.34744;29.29231;-48.03854;,
 -59.34744;8.30172;-55.45970;,
 -137.87643;8.30172;-61.34570;,
 -135.73409;-20.14310;-55.49277;,
 -59.34744;8.30172;-55.45970;,
 -59.34744;-12.68887;-48.03854;,
 -140.63707;50.14138;0.00000;,
 -140.63707;-33.53796;0.00000;,
 -59.34723;-29.67287;0.00000;,
 1.79487;77.90364;48.03854;,
 -74.59180;85.35793;55.49277;,
 -76.73416;56.91312;61.34570;,
 1.79487;56.91312;55.45970;,
 -144.27744;77.90364;48.03854;,
 -148.56212;56.91312;52.32321;,
 -74.59180;28.46850;55.49277;,
 1.79487;35.92272;48.03854;,
 -144.27744;35.92272;48.03854;,
 -154.08318;87.70941;0.00000;,
 -158.36784;56.91312;0.00000;,
 -144.27744;77.90364;-48.03854;,
 -148.56212;56.91312;-52.32302;,
 -154.08318;26.11679;0.00000;,
 -144.27744;35.92272;-48.03854;,
 -74.59180;85.35793;-55.49277;,
 -76.73416;56.91312;-61.34570;,
 1.79487;77.90364;-48.03854;,
 1.79487;56.91312;-55.45970;,
 -74.59180;28.46850;-55.49277;,
 1.79487;35.92272;-48.03854;,
 -79.49480;98.75275;0.00000;,
 -2.47701;77.23950;-48.03854;,
 73.90986;84.69370;-55.49277;,
 76.05222;56.24908;-61.34570;,
 -2.47701;56.24908;-55.45970;,
 143.59554;77.23950;-48.03854;,
 147.88016;56.24908;-52.32302;,
 73.90986;27.80429;-55.49277;,
 -2.47701;35.25850;-48.03854;,
 143.59554;35.25850;-48.03854;,
 153.40115;87.04541;0.00000;,
 157.68590;56.24908;0.00000;,
 143.59554;77.23950;48.03854;,
 147.88016;56.24908;52.32302;,
 153.40115;25.45279;0.00000;,
 143.59554;35.25850;48.03854;,
 73.90986;84.69370;55.49277;,
 76.05222;56.24908;61.34570;,
 -2.47701;77.23950;48.03854;,
 -2.47701;56.24908;55.45970;,
 73.90986;27.80429;55.49277;,
 -2.47701;35.25850;48.03854;,
 78.81286;98.08878;0.00000;,
 -77.44424;29.29231;-48.03854;,
 -1.05732;36.74631;-55.49277;,
 1.08503;8.30172;-61.34570;,
 -77.44424;8.30172;-55.45970;,
 68.62813;29.29231;-48.03854;,
 72.91277;8.30172;-52.32302;,
 -77.44424;8.30172;-55.45970;,
 1.08503;8.30172;-61.34570;,
 -1.05732;-20.14310;-55.49277;,
 -77.44424;-12.68887;-48.03854;,
 68.62813;-12.68887;-48.03854;,
 -1.05732;-20.14310;-55.49277;,
 78.43407;39.09805;0.00000;,
 82.71875;8.30172;0.00000;,
 68.62813;29.29231;48.03854;,
 72.91277;8.30172;52.32302;,
 78.43407;-22.49459;-0.00000;,
 68.62813;-12.68887;48.03854;,
 -1.05732;36.74631;55.49277;,
 1.08503;8.30172;61.34570;,
 -77.44424;29.29231;48.03854;,
 -77.44424;8.30172;55.45970;,
 -1.05732;-20.14310;55.49277;,
 68.62813;-12.68887;48.03854;,
 -77.44424;-12.68887;48.03854;,
 3.84544;-33.53796;-0.00000;,
 -77.44424;-29.67287;0.00000;,
 68.62813;-12.68887;-48.03854;,
 78.43407;-22.49459;-0.00000;,
 76.77827;108.66969;-64.47414;,
 76.77827;72.60640;-45.59009;,
 0.38945;72.60640;-45.59009;,
 0.38945;108.66969;-64.47414;,
 76.77827;72.60640;45.59009;,
 76.77827;108.66969;64.47414;,
 0.38945;108.66969;64.47414;,
 0.38945;72.60640;45.59009;,
 76.77827;144.73293;45.59006;,
 0.38945;144.73293;45.59006;,
 76.77827;159.67070;-0.00000;,
 0.38945;159.67070;-0.00000;,
 76.77827;144.73293;-45.59007;,
 0.38945;144.73293;-45.59007;,
 -75.99968;72.60643;-45.59009;,
 -75.99968;108.66969;-64.47414;,
 -75.99968;108.66972;64.47414;,
 -75.99968;72.60643;45.59009;,
 -75.99968;144.73293;45.59006;,
 -75.99962;159.67070;-0.00000;,
 -75.99968;144.73293;-45.59007;,
 96.47725;108.66969;0.00000;,
 76.77827;57.66855;0.00000;,
 -95.69865;108.66969;0.00000;,
 -75.99968;57.66855;0.00000;;
 
 108;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;3,2,6,7;,
 4;2,5,8,6;,
 4;4,9,10,5;,
 4;9,11,12,10;,
 4;5,10,13,8;,
 4;10,12,14,13;,
 4;11,15,16,12;,
 4;15,17,18,16;,
 4;12,16,19,14;,
 4;16,18,20,19;,
 4;15,11,9,21;,
 4;21,9,4,1;,
 4;22,23,24,25;,
 4;23,26,27,24;,
 4;25,24,19,20;,
 4;24,27,28,19;,
 4;29,30,31,32;,
 4;30,33,34,31;,
 4;32,31,35,36;,
 4;31,34,37,35;,
 4;33,38,39,34;,
 4;38,40,41,39;,
 4;34,39,42,37;,
 4;39,41,43,42;,
 4;40,44,45,46;,
 4;44,47,48,45;,
 4;41,49,50,43;,
 4;49,51,52,50;,
 4;44,40,38,53;,
 4;53,38,33,30;,
 4;36,35,54,55;,
 4;35,37,42,54;,
 4;55,54,50,52;,
 4;54,42,43,50;,
 4;56,57,58,59;,
 4;57,60,61,58;,
 4;59,58,62,63;,
 4;58,61,64,62;,
 4;60,65,66,61;,
 4;65,67,68,66;,
 4;61,66,69,64;,
 4;66,68,70,69;,
 4;67,71,72,68;,
 4;71,73,74,72;,
 4;68,72,75,70;,
 4;72,74,76,75;,
 4;71,67,65,77;,
 4;77,65,60,57;,
 4;78,79,80,81;,
 4;79,82,83,80;,
 4;81,80,84,85;,
 4;80,83,86,84;,
 4;82,87,88,83;,
 4;87,89,90,88;,
 4;83,88,91,86;,
 4;88,90,92,91;,
 4;89,93,94,90;,
 4;93,95,96,94;,
 4;90,94,97,92;,
 4;94,96,98,97;,
 4;93,89,87,99;,
 4;99,87,82,79;,
 4;100,101,102,103;,
 4;101,104,105,102;,
 4;106,107,108,109;,
 4;102,105,110,111;,
 4;104,112,113,105;,
 4;112,114,115,113;,
 4;105,113,116,110;,
 4;113,115,117,116;,
 4;114,118,119,115;,
 4;118,120,121,119;,
 4;115,119,122,123;,
 4;119,121,124,122;,
 4;109,108,125,126;,
 4;108,127,128,125;,
 4;126,125,122,124;,
 4;125,128,123,122;,
 4;129,130,131,132;,
 4;133,134,135,136;,
 4;134,137,138,135;,
 4;137,139,140,138;,
 4;139,141,142,140;,
 4;141,129,132,142;,
 4;132,131,143,144;,
 4;136,135,145,146;,
 4;135,138,147,145;,
 4;138,140,148,147;,
 4;140,142,149,148;,
 4;142,132,144,149;,
 3;150,130,129;,
 3;150,151,130;,
 3;150,133,151;,
 3;150,134,133;,
 3;150,137,134;,
 3;150,139,137;,
 3;150,141,139;,
 3;150,129,141;,
 3;152,144,143;,
 3;152,143,153;,
 3;152,153,146;,
 3;152,146,145;,
 3;152,145,147;,
 3;152,147,148;,
 3;152,148,149;,
 3;152,149,144;;
 
 MeshMaterialList {
  1;
  108;
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
  0;;
  Material {
   0.800000;0.784314;0.771765;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MESH\sandbag.png";
   }
  }
 }
 MeshNormals {
  204;
  -0.098716;0.263983;-0.959462;,
  0.012629;0.241391;-0.970346;,
  0.123809;0.215655;-0.968589;,
  -0.102347;0.000000;-0.994749;,
  0.013013;0.000000;-0.999915;,
  0.126793;0.000000;-0.991929;,
  -0.105050;-0.697199;-0.709139;,
  0.012629;-0.241391;-0.970346;,
  0.123809;-0.215654;-0.968589;,
  0.982332;0.187144;0.000000;,
  0.962062;0.183283;0.202101;,
  1.000000;-0.000000;-0.000000;,
  0.978640;-0.000000;0.205583;,
  0.982332;-0.187144;-0.000000;,
  0.962062;-0.183283;0.202101;,
  0.012629;0.241391;0.970346;,
  -0.098716;0.263983;0.959462;,
  0.013013;0.000000;0.999915;,
  -0.102347;0.000000;0.994749;,
  0.012629;-0.241391;0.970346;,
  -0.105050;-0.697199;0.709139;,
  0.139682;0.990196;0.000000;,
  -0.088660;-0.996062;0.000000;,
  0.026420;-0.999651;-0.000000;,
  0.962062;0.183283;-0.202101;,
  0.978640;-0.000000;-0.205583;,
  0.962062;-0.183283;-0.202101;,
  0.123809;0.215655;0.968589;,
  0.126793;0.000000;0.991929;,
  0.123809;-0.215654;0.968589;,
  0.135782;0.962551;0.234645;,
  0.135782;0.962551;-0.234645;,
  0.025514;-0.965375;-0.259614;,
  0.135784;-0.962551;-0.234644;,
  0.139683;-0.990196;-0.000000;,
  0.025514;-0.965375;0.259614;,
  0.135784;-0.962551;0.234644;,
  0.098716;0.263983;0.959462;,
  -0.012628;0.241393;0.970345;,
  -0.123807;0.215659;0.968588;,
  0.102347;0.000000;0.994749;,
  -0.013013;0.000000;0.999915;,
  -0.126791;0.000000;0.991929;,
  0.105050;-0.697198;0.709140;,
  -0.012628;-0.241393;0.970345;,
  -0.123808;-0.215658;0.968588;,
  -0.982332;0.187144;-0.000000;,
  -0.962062;0.183282;-0.202101;,
  -1.000000;-0.000000;-0.000000;,
  -0.978640;-0.000000;-0.205583;,
  -0.982332;-0.187144;-0.000000;,
  -0.962062;-0.183282;-0.202101;,
  -0.012628;0.241391;-0.970346;,
  0.098717;0.263983;-0.959462;,
  -0.013013;0.000000;-0.999915;,
  0.102347;0.000000;-0.994749;,
  -0.012628;-0.241391;-0.970346;,
  0.105050;-0.697198;-0.709140;,
  -0.139681;0.990197;0.000000;,
  0.088660;-0.996062;-0.000000;,
  -0.026420;-0.999651;-0.000000;,
  -0.962062;0.183283;0.202101;,
  -0.978640;-0.000000;0.205583;,
  -0.962062;-0.183283;0.202101;,
  -0.123809;0.215655;-0.968589;,
  -0.126792;0.000000;-0.991929;,
  -0.123809;-0.215654;-0.968589;,
  -0.135781;0.962551;-0.234646;,
  -0.135781;0.962551;0.234646;,
  -0.025514;-0.965375;0.259614;,
  -0.135783;-0.962551;0.234644;,
  -0.139683;-0.990196;0.000000;,
  -0.025514;-0.965375;-0.259614;,
  -0.135783;-0.962551;-0.234644;,
  0.098717;0.263983;0.959462;,
  -0.012627;0.241393;0.970345;,
  -0.123808;0.215659;0.968588;,
  0.102348;-0.000001;0.994749;,
  -0.013012;-0.000001;0.999915;,
  -0.126791;-0.000001;0.991929;,
  0.098717;-0.263984;0.959462;,
  -0.012628;-0.241394;0.970345;,
  -0.123808;-0.215660;0.968588;,
  -0.982332;0.187144;-0.000000;,
  -0.962062;0.183282;-0.202101;,
  -1.000000;-0.000000;-0.000000;,
  -0.978639;-0.000000;-0.205584;,
  -0.982332;-0.187145;-0.000000;,
  -0.962062;-0.183283;-0.202102;,
  -0.012628;0.241391;-0.970346;,
  0.098717;0.263983;-0.959462;,
  -0.013013;-0.000001;-0.999915;,
  0.102348;-0.000001;-0.994749;,
  -0.012628;-0.241392;-0.970345;,
  0.098717;-0.263984;-0.959462;,
  -0.139683;0.990196;0.000000;,
  -0.962062;0.183283;0.202101;,
  -0.978640;-0.000000;0.205584;,
  -0.962061;-0.183283;0.202101;,
  -0.123809;0.215655;-0.968589;,
  -0.126792;-0.000001;-0.991929;,
  -0.123809;-0.215656;-0.968589;,
  -0.135783;0.962551;-0.234644;,
  -0.135783;0.962551;0.234644;,
  -0.098717;0.263984;-0.959462;,
  0.012628;0.241392;-0.970345;,
  0.123809;0.215656;-0.968589;,
  -0.102347;0.000001;-0.994749;,
  0.013013;0.000001;-0.999915;,
  0.126792;0.000001;-0.991929;,
  -0.098717;-0.263983;-0.959463;,
  0.012628;-0.241391;-0.970346;,
  0.123809;-0.215654;-0.968589;,
  0.982332;0.187144;0.000000;,
  0.962062;0.183283;0.202101;,
  1.000000;0.000000;0.000000;,
  0.978640;0.000000;0.205583;,
  0.982333;-0.187144;-0.000000;,
  0.962062;-0.183282;0.202100;,
  0.012628;0.241392;0.970345;,
  -0.098717;0.263984;0.959462;,
  0.013013;0.000001;0.999915;,
  -0.102347;0.000001;0.994749;,
  0.012628;-0.241391;0.970346;,
  -0.098717;-0.263983;0.959463;,
  0.139683;0.990196;0.000000;,
  0.962062;0.183283;-0.202101;,
  0.978640;0.000000;-0.205583;,
  0.962062;-0.183282;-0.202100;,
  0.123809;0.215656;0.968589;,
  0.126792;0.000001;0.991929;,
  0.123809;-0.215654;0.968589;,
  0.135783;0.962551;0.234647;,
  0.135783;0.962551;-0.234647;,
  -0.098716;0.263983;-0.959462;,
  0.012629;0.241391;-0.970346;,
  -0.102347;0.000000;-0.994749;,
  0.013013;0.000000;-0.999915;,
  0.123809;0.215655;-0.968589;,
  0.126793;0.000000;-0.991929;,
  -0.105050;-0.697198;-0.709140;,
  0.012629;-0.241391;-0.970346;,
  0.123809;-0.215654;-0.968589;,
  0.982333;0.187143;0.000000;,
  1.000000;-0.000000;-0.000000;,
  0.962061;0.183282;0.202105;,
  0.978639;-0.000000;0.205588;,
  0.982333;-0.187143;-0.000000;,
  0.962061;-0.183282;0.202105;,
  0.012629;0.241391;0.970346;,
  0.013013;0.000000;0.999915;,
  -0.098716;0.263983;0.959462;,
  -0.102347;0.000000;0.994749;,
  0.012629;-0.241391;0.970346;,
  -0.105050;-0.697198;0.709140;,
  -0.088660;-0.996062;0.000000;,
  0.026420;-0.999651;-0.000000;,
  0.962061;0.183282;-0.202105;,
  0.978639;-0.000000;-0.205588;,
  0.962061;-0.183282;-0.202105;,
  0.123809;0.215655;0.968589;,
  0.126793;0.000000;0.991929;,
  0.123809;-0.215654;0.968589;,
  0.025514;-0.965375;-0.259614;,
  0.135783;-0.962551;-0.234644;,
  0.139683;-0.990196;-0.000000;,
  0.025514;-0.965375;0.259614;,
  0.135783;-0.962551;0.234644;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000001;-1.000000;,
  0.000000;-0.463887;-0.885895;,
  0.932835;-0.360304;0.000000;,
  0.000000;-0.463887;0.885895;,
  0.000000;0.000001;1.000000;,
  0.000000;0.763214;0.646146;,
  0.000000;1.000000;0.000000;,
  0.000000;0.763214;-0.646146;,
  -0.000000;0.000001;-1.000000;,
  -0.000000;-0.463887;-0.885895;,
  -0.000000;-0.463887;0.885895;,
  -0.000000;0.000001;1.000000;,
  0.000000;0.763214;0.646146;,
  0.000000;0.763214;-0.646146;,
  -0.000000;0.000001;-1.000000;,
  -0.000000;-0.463887;-0.885895;,
  -0.932835;-0.360304;0.000000;,
  -0.000000;-0.463887;0.885895;,
  -0.000000;0.000001;1.000000;,
  0.000000;0.763214;0.646146;,
  -1.000000;0.000001;0.000000;,
  0.944474;-0.256870;-0.204904;,
  0.956358;0.000000;-0.292199;,
  0.944474;-0.256870;0.204904;,
  0.956358;0.000000;0.292199;,
  0.944474;0.256871;0.204904;,
  0.932835;0.360304;0.000000;,
  0.944474;0.256871;-0.204904;,
  -0.956358;0.000000;-0.292199;,
  -0.944474;-0.256870;-0.204904;,
  -0.944474;-0.256870;0.204904;,
  -0.956358;0.000000;0.292199;,
  -0.944474;0.256871;0.204904;,
  -0.932834;0.360305;0.000000;,
  -0.944474;0.256871;-0.204904;;
  108;
  4;0,1,4,3;,
  4;1,2,5,4;,
  4;3,4,7,6;,
  4;4,5,8,7;,
  4;24,9,11,25;,
  4;9,10,12,11;,
  4;25,11,13,26;,
  4;11,12,14,13;,
  4;27,15,17,28;,
  4;15,16,18,17;,
  4;28,17,19,29;,
  4;17,18,20,19;,
  4;30,30,21,21;,
  4;21,21,31,31;,
  4;6,32,23,22;,
  4;32,33,34,23;,
  4;22,23,35,20;,
  4;23,34,36,35;,
  4;37,38,41,40;,
  4;38,39,42,41;,
  4;40,41,44,43;,
  4;41,42,45,44;,
  4;61,46,48,62;,
  4;46,47,49,48;,
  4;62,48,50,63;,
  4;48,49,51,50;,
  4;64,52,54,65;,
  4;52,53,55,54;,
  4;65,54,56,66;,
  4;54,55,57,56;,
  4;67,67,58,58;,
  4;58,58,68,68;,
  4;43,69,60,59;,
  4;69,70,71,60;,
  4;59,60,72,57;,
  4;60,71,73,72;,
  4;74,75,78,77;,
  4;75,76,79,78;,
  4;77,78,81,80;,
  4;78,79,82,81;,
  4;96,83,85,97;,
  4;83,84,86,85;,
  4;97,85,87,98;,
  4;85,86,88,87;,
  4;99,89,91,100;,
  4;89,90,92,91;,
  4;100,91,93,101;,
  4;91,92,94,93;,
  4;102,102,95,95;,
  4;95,95,103,103;,
  4;104,105,108,107;,
  4;105,106,109,108;,
  4;107,108,111,110;,
  4;108,109,112,111;,
  4;126,113,115,127;,
  4;113,114,116,115;,
  4;127,115,117,128;,
  4;115,116,118,117;,
  4;129,119,121,130;,
  4;119,120,122,121;,
  4;130,121,123,131;,
  4;121,122,124,123;,
  4;132,132,125,125;,
  4;125,125,133,133;,
  4;134,135,137,136;,
  4;135,138,139,137;,
  4;136,137,141,140;,
  4;137,139,142,141;,
  4;157,143,144,158;,
  4;143,145,146,144;,
  4;158,144,147,159;,
  4;144,146,148,147;,
  4;160,149,150,161;,
  4;149,151,152,150;,
  4;161,150,153,162;,
  4;150,152,154,153;,
  4;140,163,156,155;,
  4;163,164,165,156;,
  4;155,156,166,154;,
  4;156,165,167,166;,
  4;169,170,178,177;,
  4;172,173,180,179;,
  4;173,174,181,180;,
  4;174,175,175,181;,
  4;175,176,182,175;,
  4;176,169,177,182;,
  4;177,178,184,183;,
  4;179,180,187,186;,
  4;180,181,188,187;,
  4;181,175,175,188;,
  4;175,182,182,175;,
  4;182,177,183,182;,
  3;168,190,191;,
  3;168,171,190;,
  3;168,192,171;,
  3;168,193,192;,
  3;168,194,193;,
  3;168,195,194;,
  3;168,196,195;,
  3;168,191,196;,
  3;189,197,198;,
  3;189,198,185;,
  3;189,185,199;,
  3;189,199,200;,
  3;189,200,201;,
  3;189,201,202;,
  3;189,202,203;,
  3;189,203,197;;
 }
 MeshTextureCoords {
  154;
  0.059008;0.571335;,
  0.063446;0.707878;,
  0.018709;0.713401;,
  0.012697;0.571063;,
  0.055549;0.841142;,
  0.025232;0.850228;,
  -0.026925;0.716402;,
  -0.032699;0.577625;,
  -0.005259;0.847432;,
  0.108729;0.879907;,
  0.048161;0.892436;,
  0.203084;0.885388;,
  0.181161;0.898420;,
  -0.029824;0.889136;,
  -0.021533;0.891678;,
  0.293451;0.758990;,
  0.388511;0.769904;,
  0.356985;0.615581;,
  0.457571;0.622144;,
  0.639377;0.767514;,
  0.580336;0.621872;,
  0.144321;0.740483;,
  0.967301;0.577625;,
  0.973075;0.716402;,
  0.907627;0.753021;,
  0.854090;0.602293;,
  0.994741;0.847432;,
  0.970176;0.889136;,
  0.978467;0.891678;,
  0.442465;0.391637;,
  0.457106;0.252861;,
  0.512824;0.255861;,
  0.502524;0.398200;,
  0.484250;0.121830;,
  0.521186;0.119034;,
  0.567194;0.261384;,
  0.563226;0.397927;,
  0.557430;0.128120;,
  0.441312;0.080126;,
  0.549034;0.076826;,
  0.143975;0.077584;,
  0.834998;0.070842;,
  0.626812;0.089354;,
  0.774475;0.083874;,
  0.092892;0.201749;,
  -0.041034;0.199358;,
  -0.165002;0.070842;,
  0.066238;0.347390;,
  -0.010115;0.347118;,
  0.958966;0.199358;,
  0.856681;0.210273;,
  0.989885;0.347118;,
  0.915773;0.353680;,
  0.353561;0.216242;,
  0.674758;0.228779;,
  0.726787;0.378349;,
  0.325867;0.497288;,
  0.361977;0.358512;,
  0.406554;0.361512;,
  0.363059;0.503850;,
  0.388164;0.227481;,
  0.423689;0.224685;,
  0.455968;0.367035;,
  0.390160;0.503578;,
  0.457483;0.233771;,
  0.312109;0.185777;,
  0.354020;0.182477;,
  0.198298;0.183235;,
  0.163612;0.176493;,
  0.454038;0.195005;,
  0.116498;0.189525;,
  0.162805;0.307400;,
  0.115662;0.305009;,
  0.144480;0.453041;,
  0.107180;0.452769;,
  0.054815;0.315924;,
  0.077205;0.459332;,
  0.269609;0.321892;,
  0.144850;0.445251;,
  0.127942;0.581793;,
  0.090128;0.587317;,
  0.106923;0.444978;,
  0.117019;0.715058;,
  0.090317;0.724144;,
  0.050838;0.590317;,
  0.076191;0.451541;,
  0.065790;0.721348;,
  0.177222;0.753823;,
  0.147735;0.766352;,
  0.256452;0.759304;,
  0.266451;0.772336;,
  0.095708;0.763052;,
  0.262978;0.765594;,
  0.299540;0.632905;,
  0.332403;0.643819;,
  0.328538;0.489497;,
  0.366363;0.496059;,
  0.371651;0.641429;,
  0.394766;0.495787;,
  0.206367;0.614398;,
  0.068266;0.313964;,
  0.071097;0.450507;,
  0.003349;0.456031;,
  -0.016122;0.313692;,
  0.058775;0.583771;,
  0.014604;0.592857;,
  0.983878;0.313692;,
  1.003349;0.456031;,
  0.934779;0.459031;,
  0.903538;0.320254;,
  -0.030883;0.590061;,
  -0.065221;0.459031;,
  0.152683;0.622537;,
  0.047729;0.635066;,
  0.350048;0.628017;,
  0.443874;0.641050;,
  -0.108192;0.631766;,
  -0.417484;0.634307;,
  0.402962;0.501619;,
  0.488212;0.512533;,
  0.450023;0.358210;,
  0.505773;0.364773;,
  0.579456;0.510143;,
  0.582516;0.634307;,
  0.562130;0.364501;,
  0.787864;0.495649;,
  0.711286;0.344922;,
  0.969117;0.590061;,
  0.891808;0.631766;,
  0.139164;0.579363;,
  0.125326;0.593463;,
  0.142316;0.452367;,
  0.152603;0.438267;,
  0.286580;0.635454;,
  0.299810;0.638747;,
  0.326281;0.497651;,
  0.327295;0.494358;,
  0.268716;0.624647;,
  0.289829;0.483551;,
  0.223198;0.601413;,
  0.241093;0.460317;,
  0.176601;0.582656;,
  0.191690;0.441560;,
  0.167212;0.311270;,
  0.169825;0.297170;,
  0.351299;0.356555;,
  0.364591;0.353261;,
  0.311252;0.342455;,
  0.260639;0.319221;,
  0.209444;0.300464;,
  0.205124;0.645440;,
  0.180040;0.616696;,
  0.274276;0.290477;,
  0.283720;0.334504;;
 }
}

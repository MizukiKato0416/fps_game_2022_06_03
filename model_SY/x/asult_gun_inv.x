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
 224;
 -0.63739;1.40126;-2.44743;,
 -0.60341;-0.14136;-6.13101;,
 -0.60532;2.47455;-6.13106;,
 -0.63384;2.45954;-2.41980;,
 -0.60341;-0.14136;-6.13101;,
 0.57969;-0.14019;-6.11187;,
 0.57778;2.47570;-6.11192;,
 -0.60532;2.47455;-6.13106;,
 0.57969;-0.14019;-6.11187;,
 0.57236;1.40240;-2.42869;,
 0.57590;2.46067;-2.40106;,
 0.57778;2.47570;-6.11192;,
 0.57590;2.46067;-2.40106;,
 -0.63384;2.45954;-2.41980;,
 -0.63739;1.40126;-2.44743;,
 0.57236;1.40240;-2.42869;,
 -0.65272;1.38289;-1.45132;,
 -0.63739;1.40126;-2.44743;,
 -0.64917;2.44117;-1.42899;,
 0.55703;1.38404;-1.43258;,
 0.57236;1.40240;-2.42869;,
 0.56057;2.44231;-1.41025;,
 0.21162;0.30911;-0.22040;,
 -0.35567;0.30746;-0.22937;,
 -0.34391;-1.60626;-0.65808;,
 0.22338;-1.60461;-0.64911;,
 -0.35567;0.30746;-0.22937;,
 -0.34193;0.32967;-1.13991;,
 -0.32996;-1.60626;-1.56863;,
 -0.34391;-1.60626;-0.65808;,
 -0.34193;0.32967;-1.13991;,
 0.22537;0.33132;-1.13094;,
 0.23734;-1.60344;-1.55965;,
 -0.32996;-1.60626;-1.56863;,
 0.22537;0.33132;-1.13094;,
 0.21162;0.30911;-0.22040;,
 0.22338;-1.60461;-0.64911;,
 0.23734;-1.60344;-1.55965;,
 0.22338;-1.60461;-0.64911;,
 -0.34391;-1.60626;-0.65808;,
 -0.14528;-0.33595;0.13670;,
 -0.14529;-0.50944;0.13095;,
 -0.13563;-0.50944;-0.49281;,
 -0.13579;-0.33545;-0.49282;,
 -0.16917;0.42470;0.71419;,
 -0.17043;0.23380;0.80728;,
 -0.01920;-0.33483;0.12714;,
 -0.01886;-0.50932;0.13290;,
 -0.01609;0.23395;0.80968;,
 -0.01482;0.42485;0.71658;,
 -0.00937;-0.33533;-0.49086;,
 -0.00921;-0.50933;-0.49085;,
 -0.14528;-0.33595;0.13670;,
 -0.01920;-0.33483;0.12714;,
 -0.01482;0.42485;0.71658;,
 -0.16917;0.42470;0.71419;,
 -0.14529;-0.50944;0.13095;,
 -0.01886;-0.50932;0.13290;,
 -0.17043;0.23380;0.80728;,
 -0.01609;0.23395;0.80968;,
 -0.01609;0.23395;0.80968;,
 -0.01482;0.42485;0.71658;,
 -0.16917;0.42470;0.71419;,
 -0.17043;0.23380;0.80728;,
 -0.11927;0.27451;-0.10107;,
 -0.12021;0.27451;-0.04033;,
 -0.11887;0.11837;-0.11751;,
 -0.11792;0.11837;-0.17825;,
 -0.12044;-0.03079;-0.00712;,
 -0.11949;-0.03079;-0.06785;,
 -0.11949;-0.03079;-0.06785;,
 -0.12044;-0.03079;-0.00712;,
 -0.04172;-0.03071;-0.00590;,
 -0.04078;-0.03071;-0.06664;,
 -0.04078;-0.03071;-0.06664;,
 -0.04172;-0.03071;-0.00590;,
 -0.04015;0.11844;-0.11629;,
 -0.03922;0.11844;-0.17703;,
 -0.04149;0.27459;-0.03911;,
 -0.04056;0.27459;-0.09985;,
 -0.03922;0.11844;-0.17703;,
 -0.11792;0.11837;-0.17825;,
 -0.04056;0.27459;-0.09985;,
 -0.11927;0.27451;-0.10107;,
 -0.04015;0.11844;-0.11629;,
 -0.11887;0.11837;-0.11751;,
 -0.04149;0.27459;-0.03911;,
 -0.12021;0.27451;-0.04033;,
 0.40301;1.59215;8.25813;,
 0.37143;1.59209;6.62484;,
 0.26124;1.34953;6.62504;,
 0.29282;1.34959;8.25834;,
 -0.00522;1.24885;6.62555;,
 0.02637;1.24891;8.25885;,
 -0.27186;1.34903;6.62606;,
 -0.24028;1.34909;8.25936;,
 -0.38250;1.59138;6.62627;,
 -0.35091;1.59144;8.25957;,
 -0.27232;1.83394;6.62606;,
 -0.24074;1.83400;8.25936;,
 -0.00586;1.93462;6.62555;,
 0.02572;1.93468;8.25885;,
 0.26078;1.83444;6.62504;,
 0.29237;1.83451;8.25834;,
 0.37143;1.59209;6.62484;,
 0.40301;1.59215;8.25813;,
 0.02604;1.59180;8.25885;,
 0.02604;1.59180;8.25885;,
 0.02604;1.59180;8.25885;,
 0.02604;1.59180;8.25885;,
 0.02604;1.59180;8.25885;,
 0.02604;1.59180;8.25885;,
 0.02604;1.59180;8.25885;,
 0.02604;1.59180;8.25885;,
 -0.75062;2.46593;-1.40826;,
 -0.75062;0.24723;-1.40826;,
 0.74245;0.24723;-1.40826;,
 0.74245;2.46593;-1.40826;,
 0.74245;2.46593;-1.40826;,
 0.74245;0.24723;-1.40826;,
 0.74245;0.24723;2.92747;,
 0.74245;2.46593;2.65036;,
 0.74245;2.46593;6.64951;,
 0.74245;0.88184;6.64951;,
 -0.75062;0.88184;6.64951;,
 -0.75062;2.46593;6.64951;,
 -0.75062;2.46593;2.65036;,
 -0.75062;0.24723;2.92747;,
 -0.75062;0.24723;-1.40826;,
 -0.75062;2.46593;-1.40826;,
 -0.75062;2.46593;2.65036;,
 -0.75062;2.46593;-1.40826;,
 0.74245;2.46593;-1.40826;,
 0.74245;2.46593;2.65036;,
 -0.75062;0.24723;-1.40826;,
 -0.75062;0.24723;2.92747;,
 0.74245;0.24723;2.92747;,
 0.74245;0.24723;-1.40826;,
 -0.75062;2.46593;2.65036;,
 -0.75062;2.46593;6.64951;,
 -0.75062;0.88184;6.64951;,
 -0.75062;0.91934;2.84682;,
 -0.75062;2.46593;2.65036;,
 0.74245;2.46593;2.65036;,
 0.74245;2.46593;6.64951;,
 -0.75062;0.91934;2.84682;,
 0.74245;0.88184;6.64951;,
 0.74245;0.91934;2.84682;,
 0.74245;2.46593;2.65036;,
 0.74245;0.91934;2.84682;,
 0.74245;2.46593;2.65036;,
 -0.75062;2.46593;2.65036;,
 -0.37176;0.39555;1.84833;,
 -0.45189;-2.09273;2.45796;,
 0.36790;-2.11841;2.46090;,
 0.44803;0.36987;1.85128;,
 0.44803;0.36987;1.85128;,
 0.36790;-2.11841;2.46090;,
 0.37197;-1.87457;3.45673;,
 0.45210;0.61371;2.84710;,
 0.45210;0.61371;2.84710;,
 0.37197;-1.87457;3.45673;,
 -0.44783;-1.84889;3.45379;,
 -0.36769;0.63939;2.84416;,
 -0.36769;0.63939;2.84416;,
 -0.44783;-1.84889;3.45379;,
 -0.45189;-2.09273;2.45796;,
 -0.37176;0.39555;1.84833;,
 -0.45189;-2.09273;2.45796;,
 0.36790;-2.11841;2.46090;,
 -0.67649;3.64218;0.74253;,
 -0.55315;3.55314;0.74191;,
 0.53719;3.55302;0.74368;,
 0.64829;3.64216;0.74253;,
 -0.67649;3.64218;0.74253;,
 -0.67050;2.62363;0.74253;,
 -0.55312;2.62363;0.74253;,
 0.53623;2.62356;0.74410;,
 0.64843;2.62363;0.74253;,
 0.64829;3.64216;0.74253;,
 0.64829;3.64216;0.74253;,
 0.64843;2.62363;0.74253;,
 0.64843;2.62363;1.11481;,
 0.64829;3.64219;1.11481;,
 0.64829;3.64219;1.11481;,
 0.53724;3.55393;1.11481;,
 -0.55150;3.55294;1.11340;,
 -0.67649;3.64218;1.11481;,
 0.64829;3.64219;1.11481;,
 0.64843;2.62363;1.11481;,
 0.53724;2.62363;1.11481;,
 -0.55310;2.62363;1.11483;,
 -0.67050;2.62363;1.11481;,
 -0.67649;3.64218;1.11481;,
 -0.67649;3.64218;1.11481;,
 -0.67050;2.62363;1.11481;,
 -0.67050;2.62363;0.74253;,
 -0.67649;3.64218;0.74253;,
 -0.67649;3.64218;1.11481;,
 -0.67649;3.64218;0.74253;,
 0.64829;3.64216;0.74253;,
 0.64829;3.64219;1.11481;,
 0.53623;2.62356;0.74410;,
 0.53719;3.55302;0.74368;,
 -0.55310;2.62363;1.11483;,
 -0.55150;3.55294;1.11340;,
 -0.79645;2.68152;-0.78634;,
 -0.79645;2.16889;-0.78634;,
 0.76795;2.16889;-0.78634;,
 0.76795;2.68152;-0.78634;,
 0.76795;2.68152;-0.78634;,
 0.76795;2.16889;-0.78634;,
 0.76795;2.16889;1.25396;,
 0.76795;2.68152;1.25396;,
 0.76795;2.68152;1.25396;,
 0.76795;2.16889;1.25396;,
 -0.79645;2.16889;1.25396;,
 -0.79645;2.68152;1.25396;,
 -0.79645;2.68152;1.25396;,
 -0.79645;2.16889;1.25396;,
 -0.79645;2.16889;-0.78634;,
 -0.79645;2.68152;-0.78634;,
 -0.79645;2.68152;-0.78634;,
 0.76795;2.68152;-0.78634;;
 
 121;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,2,11;,
 4;8,1,14,15;,
 4;16,17,13,18;,
 4;19,20,17,16;,
 4;21,12,20,19;,
 4;18,13,12,21;,
 4;22,23,24,25;,
 4;26,27,28,29;,
 4;30,31,32,33;,
 4;34,35,36,37;,
 4;38,39,28,37;,
 4;25,24,23,22;,
 4;29,28,27,26;,
 4;33,32,31,30;,
 4;37,36,35,34;,
 4;37,28,39,38;,
 4;40,41,42,43;,
 4;40,44,45,41;,
 4;46,47,48,49;,
 4;46,50,51,47;,
 4;52,53,54,55;,
 4;52,43,50,53;,
 4;56,57,51,42;,
 4;56,58,59,57;,
 4;60,61,62,63;,
 4;41,40,43,42;,
 4;44,40,41,45;,
 4;47,46,49,48;,
 4;50,46,47,51;,
 4;53,52,55,54;,
 4;43,52,53,50;,
 4;57,56,42,51;,
 4;58,56,57,59;,
 4;64,65,66,67;,
 4;67,66,68,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;77,76,78,79;,
 4;74,80,81,69;,
 4;80,82,83,81;,
 4;84,75,68,85;,
 4;86,84,85,87;,
 4;88,89,90,91;,
 4;91,90,92,93;,
 4;93,92,94,95;,
 4;95,94,96,97;,
 4;97,96,98,99;,
 4;99,98,100,101;,
 4;101,100,102,103;,
 4;103,102,104,105;,
 3;106,88,91;,
 3;107,91,93;,
 3;108,93,95;,
 3;109,95,97;,
 3;110,97,99;,
 3;111,99,101;,
 3;112,101,103;,
 3;113,103,105;,
 4;65,64,67,66;,
 4;66,67,69,68;,
 4;71,70,73,72;,
 4;75,74,77,76;,
 4;76,77,79,78;,
 4;80,74,69,81;,
 4;82,80,81,83;,
 4;75,84,85,68;,
 4;84,86,87,85;,
 4;89,88,91,90;,
 4;90,91,93,92;,
 4;92,93,95,94;,
 4;94,95,97,96;,
 4;96,97,99,98;,
 4;98,99,101,100;,
 4;100,101,103,102;,
 4;102,103,105,104;,
 3;88,106,91;,
 3;91,107,93;,
 3;93,108,95;,
 3;95,109,97;,
 3;97,110,99;,
 3;99,111,101;,
 3;101,112,103;,
 3;103,113,105;,
 4;114,115,116,117;,
 4;118,119,120,121;,
 4;122,123,124,125;,
 4;126,127,128,129;,
 4;130,131,132,133;,
 4;134,135,136,137;,
 4;138,139,140,141;,
 4;142,143,144,139;,
 4;145,140,146,147;,
 4;145,147,136,135;,
 4;148,149,146,144;,
 3;150,120,149;,
 3;151,141,127;,
 4;152,153,154,155;,
 4;156,157,158,159;,
 4;160,161,162,163;,
 4;164,165,166,167;,
 4;168,165,158,169;,
 4;170,171,172,173;,
 4;174,175,176,171;,
 4;172,177,178,179;,
 4;180,181,182,183;,
 4;184,185,186,187;,
 4;188,189,190,185;,
 4;186,191,192,193;,
 4;194,195,196,197;,
 4;198,199,200,201;,
 4;185,190,202,203;,
 4;171,176,204,205;,
 4;185,203,171,205;,
 4;206,207,208,209;,
 4;210,211,212,213;,
 4;214,215,216,217;,
 4;218,219,220,221;,
 4;218,222,223,213;;
 
 MeshMaterialList {
  5;
  121;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.545600;0.545600;0.545600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.102300;0.102300;0.102300;;
  }
  Material {
   0.702400;0.432800;0.344800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "TEXTURE\\MESH\\gun.jpg";
   }
  }
 }
 MeshNormals {
  137;
  -0.999963;0.002511;-0.008225;,
  0.999927;-0.002564;0.011827;,
  0.999965;-0.001412;0.008294;,
  -0.016176;0.000007;0.999869;,
  -0.999875;0.003707;-0.015361;,
  -0.001232;0.999830;0.018415;,
  0.999875;-0.003717;0.015360;,
  -0.999999;0.001314;-0.001089;,
  0.001121;-0.999936;-0.011279;,
  0.001017;-0.999991;-0.004043;,
  0.005226;0.922374;-0.386263;,
  0.002041;0.982175;-0.187957;,
  0.001225;-0.999828;-0.018513;,
  -0.014792;-0.218664;0.975688;,
  -0.999880;-0.002769;-0.015238;,
  0.014592;0.216339;-0.976209;,
  0.999880;0.002767;0.015251;,
  0.003949;-0.999992;-0.000582;,
  0.014792;0.218664;-0.975688;,
  0.999880;0.002769;0.015238;,
  -0.014592;-0.216339;0.976209;,
  -0.999880;-0.002767;-0.015251;,
  -0.003949;0.999992;0.000582;,
  -0.999994;0.002280;0.002656;,
  0.999397;0.003462;0.034541;,
  0.999883;0.000175;0.015279;,
  -0.999876;-0.001697;-0.015686;,
  -0.999979;0.000292;-0.006515;,
  0.999688;0.001818;0.024912;,
  0.017635;-0.896844;0.441995;,
  0.026750;-0.608871;0.792818;,
  0.004885;-0.999988;0.000081;,
  0.005409;0.914655;-0.404199;,
  -0.000909;1.000000;-0.000022;,
  0.010804;0.673195;-0.739386;,
  -0.014371;0.438353;0.898688;,
  -0.999688;-0.001818;-0.024912;,
  -0.999883;-0.000175;-0.015279;,
  -0.999397;-0.003462;-0.034541;,
  0.999979;-0.000292;0.006515;,
  0.999994;-0.002280;-0.002656;,
  0.999876;0.001697;0.015686;,
  -0.017635;0.896844;-0.441995;,
  -0.026750;0.608871;-0.792818;,
  -0.004885;0.999988;-0.000081;,
  -0.005409;-0.914655;0.404199;,
  0.000909;-1.000000;0.000022;,
  -0.010804;-0.673195;0.739386;,
  -0.999882;-0.001014;-0.015309;,
  0.999879;0.000924;0.015556;,
  0.999878;0.000987;0.015598;,
  0.999877;0.001051;0.015640;,
  -0.007833;0.893015;0.449959;,
  -0.999882;-0.000957;-0.015351;,
  -0.007860;0.893010;0.449968;,
  -0.001912;-0.000002;-0.999998;,
  -0.999813;-0.000936;0.019334;,
  -0.683617;0.729722;0.013192;,
  -0.000939;1.000000;-0.000019;,
  0.682256;0.730994;-0.013220;,
  0.999813;0.000949;-0.019335;,
  0.683633;-0.729707;-0.013193;,
  0.000944;-1.000000;0.000021;,
  -0.682241;-0.731008;0.013222;,
  0.000000;-0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.999951;0.009861;,
  -0.999881;-0.000900;-0.015392;,
  -0.012973;0.594952;0.803657;,
  -0.013472;-0.443163;0.896340;,
  0.013022;-0.594934;-0.803669;,
  0.007860;-0.893010;-0.449968;,
  0.013469;0.443162;-0.896340;,
  -0.001911;-0.000002;-0.999998;,
  -0.001913;0.000001;-0.999998;,
  -0.001913;-0.000002;-0.999998;,
  -0.001911;-0.000003;-0.999998;,
  -0.001911;-0.000002;-0.999998;,
  -0.001911;-0.000001;-0.999998;,
  -0.001913;-0.000002;-0.999998;,
  -0.001913;-0.000004;-0.999998;,
  -0.999879;-0.000924;-0.015556;,
  -0.999878;-0.000987;-0.015598;,
  -0.999877;-0.001051;-0.015640;,
  0.007833;-0.893015;-0.449959;,
  0.999881;0.000900;0.015392;,
  0.999882;0.000957;0.015351;,
  0.999882;0.001014;0.015309;,
  0.012973;-0.594952;-0.803657;,
  0.013472;0.443163;-0.896340;,
  -0.013022;0.594934;0.803669;,
  -0.013469;-0.443162;0.896340;,
  0.999813;0.000936;-0.019334;,
  0.683617;-0.729722;-0.013192;,
  0.000939;-1.000000;0.000019;,
  -0.682256;-0.730994;0.013220;,
  -0.999813;-0.000949;0.019335;,
  -0.683633;0.729707;0.013193;,
  -0.000944;1.000000;-0.000021;,
  0.682241;0.731008;-0.013222;,
  0.001911;0.000002;0.999998;,
  0.001912;0.000002;0.999998;,
  0.001913;-0.000001;0.999998;,
  0.001913;0.000002;0.999998;,
  0.001911;0.000003;0.999998;,
  0.001911;0.000002;0.999998;,
  0.001911;0.000001;0.999998;,
  0.001913;0.000002;0.999998;,
  0.001913;0.000004;0.999998;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.119140;-0.992877;,
  0.003961;0.237839;0.971297;,
  -0.999503;0.031310;-0.003581;,
  -0.003967;-0.237840;-0.971296;,
  0.999503;-0.031310;0.003587;,
  0.031263;0.970801;-0.237839;,
  0.000857;0.002163;0.999997;,
  0.005679;0.002099;0.999982;,
  0.002525;0.000348;0.999997;,
  0.012169;0.000219;0.999926;,
  -1.000000;-0.000134;0.000000;,
  -0.002465;0.003140;-0.999992;,
  0.000324;0.003542;-0.999994;,
  0.000000;0.000000;-1.000000;,
  -0.005579;-0.000803;-0.999984;,
  0.000000;-0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.999983;0.005876;0.000000;,
  -0.000004;-1.000000;0.000040;,
  0.999999;-0.000514;-0.001426;,
  -0.999997;0.000845;0.002250;,
  -0.000398;1.000000;-0.000955;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  121;
  4;1,2,2,1;,
  4;3,3,3,3;,
  4;7,0,0,7;,
  4;8,8,9,9;,
  4;10,10,11,11;,
  4;6,1,1,6;,
  4;5,11,11,5;,
  4;4,0,0,4;,
  4;12,8,8,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;28,28,25,25;,
  4;28,24,24,28;,
  4;27,27,23,23;,
  4;27,26,26,27;,
  4;29,29,30,30;,
  4;29,31,31,29;,
  4;32,32,33,33;,
  4;32,34,34,32;,
  4;35,35,35,35;,
  4;36,36,37,37;,
  4;38,36,36,38;,
  4;39,39,40,40;,
  4;41,39,39,41;,
  4;42,42,43,43;,
  4;44,42,42,44;,
  4;45,45,46,46;,
  4;47,45,45,47;,
  4;49,49,50,50;,
  4;50,50,51,51;,
  4;52,54,54,52;,
  4;69,69,53,53;,
  4;53,53,48,48;,
  4;52,70,70,52;,
  4;71,71,71,71;,
  4;72,73,73,72;,
  4;74,74,74,74;,
  4;56,56,57,57;,
  4;57,57,58,58;,
  4;58,58,59,59;,
  4;59,59,60,60;,
  4;60,60,61,61;,
  4;61,61,62,62;,
  4;62,62,63,63;,
  4;63,63,56,56;,
  3;55,75,76;,
  3;55,76,77;,
  3;55,77,78;,
  3;55,78,79;,
  3;55,79,80;,
  3;55,80,81;,
  3;55,81,82;,
  3;55,82,75;,
  4;83,83,84,84;,
  4;84,84,85,85;,
  4;73,86,86,73;,
  4;87,87,88,88;,
  4;88,88,89,89;,
  4;90,86,86,90;,
  4;91,91,91,91;,
  4;54,92,92,54;,
  4;93,93,93,93;,
  4;94,94,95,95;,
  4;95,95,96,96;,
  4;96,96,97,97;,
  4;97,97,98,98;,
  4;98,98,99,99;,
  4;99,99,100,100;,
  4;100,100,101,101;,
  4;101,101,94,94;,
  3;102,103,104;,
  3;104,103,105;,
  3;105,103,106;,
  3;106,103,107;,
  3;107,103,108;,
  3;108,103,109;,
  3;109,103,110;,
  3;110,103,102;,
  4;64,64,64,64;,
  4;65,65,65,65;,
  4;66,66,66,66;,
  4;67,67,67,67;,
  4;111,111,111,111;,
  4;112,112,112,112;,
  4;67,67,67,67;,
  4;111,111,111,111;,
  4;68,68,68,68;,
  4;113,113,113,113;,
  4;65,65,65,65;,
  3;65,65,65;,
  3;67,67,67;,
  4;114,114,114,114;,
  4;115,115,115,115;,
  4;116,116,116,116;,
  4;117,117,117,117;,
  4;118,118,118,118;,
  4;119,119,120,120;,
  4;119,121,121,119;,
  4;120,122,122,120;,
  4;123,123,123,123;,
  4;125,125,124,124;,
  4;125,126,126,125;,
  4;124,127,127,124;,
  4;130,130,130,130;,
  4;131,131,131,131;,
  4;132,132,132,132;,
  4;133,133,133,133;,
  4;134,134,134,134;,
  4;128,128,128,128;,
  4;129,129,129,129;,
  4;126,126,126,126;,
  4;135,135,135,135;,
  4;136,136,136,136;;
 }
 MeshTextureCoords {
  224;
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
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
  0.502500;0.000000;,
  0.502500;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.497500;0.000000;,
  0.497500;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;0.497500;,
  0.000000;0.497500;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.502500;,
  0.000000;0.502500;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.625000;1.000000;,
  0.625000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.875000;1.000000;,
  0.875000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.418370;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.418370;1.000000;,
  0.000000;0.418370;,
  1.000000;0.418370;,
  1.000000;0.000000;,
  0.000000;0.581630;,
  1.000000;1.000000;,
  1.000000;0.581630;,
  0.581630;0.000000;,
  0.581630;1.000000;,
  0.540810;0.000000;,
  0.459190;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.500000;,
  0.666670;0.500000;,
  0.666670;0.000000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  0.333330;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.333330;0.000000;,
  0.333330;0.500000;,
  0.666670;0.500000;,
  0.666670;0.000000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  0.333330;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.666670;1.000000;,
  0.666670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.500000;,
  0.333330;1.000000;,
  0.333330;0.500000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;;
 }
}

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
 116;
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
 -0.74598;3.80113;0.05390;,
 -0.60970;3.70565;0.05328;,
 0.59501;3.70552;0.05505;,
 0.71776;3.80111;0.05390;,
 -0.74598;3.80113;0.05390;,
 -0.73936;2.70892;0.05390;,
 -0.60967;2.70892;0.05390;,
 0.59395;2.70884;0.05547;,
 0.71792;2.70892;0.05390;,
 0.71776;3.80111;0.05390;,
 0.71776;3.80111;0.05390;,
 0.71792;2.70892;0.05390;,
 0.71792;2.70892;0.57642;,
 0.71776;3.80114;0.57642;,
 0.71776;3.80114;0.57642;,
 0.59507;3.70650;0.57642;,
 -0.60788;3.70544;0.57501;,
 -0.74598;3.80113;0.57642;,
 0.71776;3.80114;0.57642;,
 0.71792;2.70892;0.57642;,
 0.59507;2.70892;0.57642;,
 -0.60965;2.70892;0.57644;,
 -0.73936;2.70892;0.57642;,
 -0.74598;3.80113;0.57642;,
 -0.74598;3.80113;0.57642;,
 -0.73936;2.70892;0.57642;,
 -0.73936;2.70892;0.05390;,
 -0.74598;3.80113;0.05390;,
 -0.74598;3.80113;0.57642;,
 -0.74598;3.80113;0.05390;,
 0.71776;3.80111;0.05390;,
 0.71776;3.80114;0.57642;,
 0.59395;2.70884;0.05547;,
 0.59501;3.70552;0.05505;,
 -0.60965;2.70892;0.57644;,
 -0.60788;3.70544;0.57501;,
 -0.79645;2.77219;-1.32473;,
 -0.79645;2.25956;-1.32473;,
 0.76795;2.25956;-1.32473;,
 0.76795;2.77219;-1.32473;,
 0.76795;2.77219;-1.32473;,
 0.76795;2.25956;-1.32473;,
 0.76795;2.25956;0.71557;,
 0.76795;2.77219;0.71557;,
 0.76795;2.77219;0.71557;,
 0.76795;2.25956;0.71557;,
 -0.79645;2.25956;0.71557;,
 -0.79645;2.77219;0.71557;,
 -0.79645;2.77219;0.71557;,
 -0.79645;2.25956;0.71557;,
 -0.79645;2.25956;-1.32473;,
 -0.79645;2.77219;-1.32473;,
 -0.79645;2.77219;-1.32473;,
 0.76795;2.77219;-1.32473;;
 
 48;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;6,7,8,9;,
 4;10,11,12,13;,
 4;13,12,14,15;,
 4;10,16,17,5;,
 4;16,18,19,17;,
 4;20,11,4,21;,
 4;22,20,21,23;,
 4;1,0,3,2;,
 4;2,3,5,4;,
 4;7,6,9,8;,
 4;11,10,13,12;,
 4;12,13,15,14;,
 4;16,10,5,17;,
 4;18,16,17,19;,
 4;11,20,21,4;,
 4;20,22,23,21;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,49;,
 4;55,50,56,57;,
 4;55,57,46,45;,
 4;58,59,56,54;,
 3;60,30,59;,
 3;61,51,37;,
 4;62,63,64,65;,
 4;66,67,68,63;,
 4;64,69,70,71;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,81,82,77;,
 4;78,83,84,85;,
 4;86,87,88,89;,
 4;90,91,92,93;,
 4;77,82,94,95;,
 4;63,68,96,97;,
 4;77,95,63,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;106,107,108,109;,
 4;110,111,112,113;,
 4;110,114,115,105;;
 
 MeshMaterialList {
  5;
  48;
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
    "C:\\Users\\student\\Pictures\\3�N_FPS\\���f���ɒ���\\gun.jpg";
   }
  }
 }
 MeshNormals {
  50;
  -0.999882;-0.001014;-0.015309;,
  0.999879;0.000924;0.015556;,
  0.999878;0.000987;0.015598;,
  0.999877;0.001051;0.015640;,
  -0.007833;0.893015;0.449959;,
  -0.999882;-0.000957;-0.015351;,
  -0.007860;0.893010;0.449968;,
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
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.119140;-0.992877;,
  0.000775;0.002018;0.999998;,
  0.005141;0.001958;0.999985;,
  0.002285;0.000325;0.999997;,
  0.011015;0.000205;0.999939;,
  -1.000000;-0.000142;0.000000;,
  -0.002231;0.002929;-0.999993;,
  0.000293;0.003303;-0.999995;,
  0.000000;0.000000;-1.000000;,
  -0.005049;-0.000749;-0.999987;,
  0.000000;-0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.999982;0.006059;0.000000;,
  -0.000004;-1.000000;0.000030;,
  0.999999;-0.000533;-0.001124;,
  -0.999998;0.000871;0.001768;,
  -0.000387;1.000000;-0.000729;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  48;
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;4,6,6,4;,
  4;12,12,5,5;,
  4;5,5,0,0;,
  4;4,13,13,4;,
  4;14,14,14,14;,
  4;15,16,16,15;,
  4;17,17,17,17;,
  4;18,18,19,19;,
  4;19,19,20,20;,
  4;16,21,21,16;,
  4;22,22,23,23;,
  4;23,23,24,24;,
  4;25,21,21,25;,
  4;26,26,26,26;,
  4;6,27,27,6;,
  4;28,28,28,28;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;29,29,29,29;,
  4;30,30,30,30;,
  4;10,10,10,10;,
  4;29,29,29,29;,
  4;11,11,11,11;,
  4;31,31,31,31;,
  4;8,8,8,8;,
  3;8,8,8;,
  3;10,10,10;,
  4;32,32,33,33;,
  4;32,34,34,32;,
  4;33,35,35,33;,
  4;36,36,36,36;,
  4;38,38,37,37;,
  4;38,39,39,38;,
  4;37,40,40,37;,
  4;43,43,43,43;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;41,41,41,41;,
  4;42,42,42,42;,
  4;39,39,39,39;,
  4;48,48,48,48;,
  4;49,49,49,49;;
 }
 MeshTextureCoords {
  116;
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
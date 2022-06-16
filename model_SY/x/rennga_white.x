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
 73;
 12.35603;10.14845;3.59713;,
 17.04099;10.14845;3.59713;,
 17.04099;10.14845;-3.59713;,
 12.35603;10.14845;-3.59713;,
 17.04099;9.51850;-3.59713;,
 16.64625;0.72430;-2.99097;,
 12.75076;0.72430;-2.99097;,
 12.35603;9.51850;-3.59713;,
 12.35603;10.14845;-3.59713;,
 17.04099;10.14845;-3.59713;,
 17.04099;9.51850;3.59713;,
 16.64625;0.72430;2.99097;,
 16.64625;0.72430;-2.99097;,
 17.04099;9.51850;-3.59713;,
 17.04099;10.14845;-3.59713;,
 12.35603;9.51850;3.59713;,
 12.75076;0.72430;2.99097;,
 16.64625;0.72430;2.99097;,
 17.04099;9.51850;3.59713;,
 17.04099;10.14845;3.59713;,
 12.35603;10.14845;3.59713;,
 12.35603;9.51850;-3.59713;,
 12.75076;0.72430;-2.99097;,
 12.75076;0.72430;2.99097;,
 12.35603;9.51850;3.59713;,
 12.35603;10.14845;-3.59713;,
 -7.54233;6.66342;3.09724;,
 13.16827;6.66342;3.09724;,
 13.16827;6.66342;-3.09724;,
 -7.54233;6.66342;-3.09724;,
 13.16827;6.09609;-3.09724;,
 12.81298;0.87540;-2.31923;,
 -7.18703;0.87540;-2.31923;,
 -7.54233;6.09609;-3.09724;,
 -7.54233;6.66342;-3.09724;,
 13.16827;6.66342;-3.09724;,
 -7.54233;6.09609;3.09724;,
 -7.18703;0.87540;2.31923;,
 12.81298;0.87540;2.31923;,
 13.16827;6.09609;3.09724;,
 13.16827;6.66342;3.09724;,
 -7.54233;6.66342;3.09724;,
 -7.54233;6.09609;-3.09724;,
 -7.18703;0.87540;-2.31923;,
 -7.18703;0.87540;2.31923;,
 -7.54233;6.09609;3.09724;,
 -7.54233;6.66342;-3.09724;,
 -11.47297;10.14135;3.51609;,
 -6.89357;10.14135;3.51609;,
 -6.89357;10.14135;-3.51609;,
 -11.47297;10.14135;-3.51609;,
 -6.89357;9.52560;-3.51609;,
 -7.23552;0.72430;-2.99097;,
 -11.13101;0.72430;-2.99097;,
 -11.47297;9.52560;-3.51609;,
 -11.47297;10.14135;-3.51609;,
 -6.89357;10.14135;-3.51609;,
 -6.89357;9.52560;3.51609;,
 -7.23552;0.72430;2.99097;,
 -7.23552;0.72430;-2.99097;,
 -6.89357;9.52560;-3.51609;,
 -6.89357;10.14135;-3.51609;,
 -11.47297;9.52560;3.51609;,
 -11.13101;0.72430;2.99097;,
 -7.23552;0.72430;2.99097;,
 -6.89357;9.52560;3.51609;,
 -6.89357;10.14135;3.51609;,
 -11.47297;10.14135;3.51609;,
 -11.47297;9.52560;-3.51609;,
 -11.13101;0.72430;-2.99097;,
 -11.13101;0.72430;2.99097;,
 -11.47297;9.52560;3.51609;,
 -11.47297;10.14135;-3.51609;;
 
 25;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;4,7,8,9;,
 4;10,11,12,13;,
 4;10,13,14,1;,
 4;15,16,17,18;,
 4;15,18,19,20;,
 4;21,22,23,24;,
 4;21,24,0,25;,
 4;26,27,28,29;,
 4;30,31,32,33;,
 4;30,33,34,35;,
 4;36,37,38,39;,
 4;36,39,40,41;,
 4;42,43,44,45;,
 4;42,45,26,46;,
 4;47,48,49,50;,
 4;51,52,53,54;,
 4;51,54,55,56;,
 4;57,58,59,60;,
 4;57,60,61,48;,
 4;62,63,64,65;,
 4;62,65,66,67;,
 4;68,69,70,71;,
 4;68,71,47,72;;
 
 MeshMaterialList {
  1;
  25;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MESH\\rennga_white.png";
   }
  }
 }
 MeshNormals {
  36;
  0.000000;1.000000;0.000000;,
  0.000000;-0.068764;-0.997633;,
  0.998994;-0.044841;-0.000000;,
  0.000000;-0.068764;0.997633;,
  0.000000;-0.034402;-0.999408;,
  0.999749;-0.022426;-0.000000;,
  0.000000;-0.034402;0.999408;,
  0.000000;0.000001;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000001;1.000000;,
  -0.999749;-0.022426;0.000000;,
  -0.998994;-0.044841;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.147397;-0.989077;,
  0.000000;-0.147397;0.989077;,
  0.000000;-0.073901;-0.997266;,
  0.000000;-0.073901;0.997266;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.999423;-0.033969;0.000000;,
  -0.997692;-0.067898;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.059557;-0.998225;,
  0.999246;-0.038824;-0.000000;,
  0.000000;-0.059557;0.998225;,
  0.000000;-0.029792;-0.999556;,
  0.999811;-0.019415;-0.000000;,
  0.000000;-0.029792;0.999556;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.999811;-0.019416;0.000000;,
  -0.999246;-0.038824;0.000000;,
  -1.000000;0.000000;0.000000;;
  25;
  4;0,0,0,0;,
  4;4,1,1,4;,
  4;4,4,7,7;,
  4;5,2,2,5;,
  4;5,5,8,8;,
  4;6,3,3,6;,
  4;6,6,9,9;,
  4;10,11,11,10;,
  4;10,10,12,12;,
  4;13,13,13,13;,
  4;16,14,14,16;,
  4;16,16,18,18;,
  4;17,15,15,17;,
  4;17,17,19,19;,
  4;20,21,21,20;,
  4;20,20,22,22;,
  4;23,23,23,23;,
  4;27,24,24,27;,
  4;27,27,30,30;,
  4;28,25,25,28;,
  4;28,28,31,31;,
  4;29,26,26,29;,
  4;29,29,32,32;,
  4;33,34,34,33;,
  4;33,33,35,35;;
 }
 MeshTextureCoords {
  73;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  0.000000;0.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.065569;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.065569;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.065569;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.065569;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.065569;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.065569;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  0.000000;0.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.055895;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.055895;,
  1.000000;0.000000;;
 }
}

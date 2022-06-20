xof 0303txt 0032

template AnimTicksPerSecond {
  <9E415A43-7BA6-4a73-8743-B73D47E88476>
  DWORD AnimTicksPerSecond;
}

template XSkinMeshHeader {
  <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
  WORD nMaxSkinWeightsPerVertex;
  WORD nMaxSkinWeightsPerFace;
  WORD nBones;
}

template SkinWeights {
  <6f0d123b-bad2-4167-a0d0-80224f25fabb>
  STRING transformNodeName;
  DWORD nWeights;
  array DWORD vertexIndices[nWeights];
  array float weights[nWeights];
  Matrix4x4 matrixOffset;
}

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 1.000000, 0.000000,
     0.000000, 1.000000,-0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame Armature {
    FrameTransformMatrix {
      -32.248360,-0.001327, 0.000000, 0.000000,
       0.001327,-32.248360, 0.000000, 0.000000,
       0.000000, 0.000000,32.248360, 0.000000,
      -0.674652,16.786222,65.401672, 1.000000;;
    }
    Frame Armature_Bone_L_001 {
      FrameTransformMatrix {
         0.486737, 0.069706, 0.870763, 0.000000,
         0.323138, 0.911736,-0.253612, 0.000000,
        -0.811584, 0.404819, 0.421251, 0.000000,
        -0.627410, 0.724446, 0.696274, 1.000000;;
      }
      Frame Armature_Bone_L_002 {
        FrameTransformMatrix {
           0.794373,-0.598972, 0.101013, 0.000000,
           0.543891, 0.627328,-0.557354, 0.000000,
           0.270471, 0.497687, 0.824108, 0.000000,
          -0.000000, 0.577348, 0.000000, 1.000000;;
        }
        Frame Armature_Bone_L_003 {
          FrameTransformMatrix {
             0.952435,-0.303059,-0.031985, 0.000000,
             0.296523, 0.945841,-0.132130, 0.000000,
             0.070296, 0.116361, 0.990716, 0.000000,
             0.000000, 0.470102,-0.000000, 1.000000;;
          }
        } //End of Armature_Bone_L_003
      } //End of Armature_Bone_L_002
    } //End of Armature_Bone_L_001
    Frame Armature_Bone_R_001 {
      FrameTransformMatrix {
         0.379451,-0.806062,-0.454182, 0.000000,
         0.536020, 0.591643,-0.602197, 0.000000,
         0.754122,-0.014946, 0.656565, 0.000000,
         0.190389,-0.266597, 0.906023, 1.000000;;
      }
      Frame Armature_Bone_R_002 {
        FrameTransformMatrix {
           0.540569, 0.819433,-0.190565, 0.000000,
          -0.841257, 0.524206,-0.132268, 0.000000,
          -0.008489, 0.231814, 0.972723, 0.000000,
          -0.000000, 0.577348,-0.000000, 1.000000;;
        }
        Frame Armature_Bone_R_003 {
          FrameTransformMatrix {
             0.924988, 0.240328,-0.294346, 0.000000,
            -0.283390, 0.952323,-0.113008, 0.000000,
             0.253153, 0.187946, 0.948994, 0.000000,
             0.000000, 0.470102,-0.000000, 1.000000;;
          }
        } //End of Armature_Bone_R_003
      } //End of Armature_Bone_R_002
    } //End of Armature_Bone_R_001
    Frame armL {
      FrameTransformMatrix {
         0.864408, 0.000000, 0.000000, 0.000000,
         0.000000, 0.404646, 0.000000, 0.000000,
         0.000000,-0.000000, 0.437146, 0.000000,
         0.281983,-0.003787,-0.957814, 1.000000;;
      }
      Mesh { //armL_001 Mesh
        56;
        -2.051001; 0.263054; 4.622179;,
        -2.051002;-0.290787; 4.622179;,
        -2.051002;-0.290788; 4.235924;,
        -2.051002; 0.263054; 4.235924;,
        -0.756924;-0.378827; 4.174525;,
        -0.756924;-0.378826; 4.683576;,
        -0.756923; 0.351089; 4.683576;,
        -0.756924; 0.351089; 4.174525;,
        -1.314101;-0.340921; 4.200961;,
        -0.756924;-0.378827; 4.174525;,
        -0.756924; 0.351089; 4.174525;,
        -1.314101; 0.313185; 4.200961;,
        -1.314101;-0.340920; 4.657141;,
        -0.756924;-0.378826; 4.683576;,
        -0.756924;-0.378827; 4.174525;,
        -1.314101;-0.340921; 4.200961;,
        -1.314100; 0.313185; 4.657141;,
        -0.756923; 0.351089; 4.683576;,
        -0.756924;-0.378826; 4.683576;,
        -1.314101;-0.340920; 4.657141;,
        -0.756923; 0.351089; 4.683576;,
        -1.314100; 0.313185; 4.657141;,
        -1.314101; 0.313185; 4.200961;,
        -0.756924; 0.351089; 4.174525;,
        -2.051002;-0.290788; 4.235924;,
        -1.468462;-0.330419; 4.208285;,
        -1.468462; 0.302684; 4.208285;,
        -2.051002; 0.263054; 4.235924;,
        -1.468462;-0.330419; 4.208285;,
        -1.314101;-0.340921; 4.200961;,
        -1.314101; 0.313185; 4.200961;,
        -1.468462; 0.302684; 4.208285;,
        -2.051002;-0.290787; 4.622179;,
        -1.468462;-0.330419; 4.649817;,
        -1.468462;-0.330419; 4.208285;,
        -2.051002;-0.290788; 4.235924;,
        -1.468462;-0.330419; 4.649817;,
        -1.314101;-0.340920; 4.657141;,
        -1.314101;-0.340921; 4.200961;,
        -1.468462;-0.330419; 4.208285;,
        -2.051001; 0.263054; 4.622179;,
        -1.468461; 0.302684; 4.649817;,
        -1.468462;-0.330419; 4.649817;,
        -2.051002;-0.290787; 4.622179;,
        -1.468461; 0.302684; 4.649817;,
        -1.314100; 0.313185; 4.657141;,
        -1.314101;-0.340920; 4.657141;,
        -1.468462;-0.330419; 4.649817;,
        -1.314100; 0.313185; 4.657141;,
        -1.468461; 0.302684; 4.649817;,
        -1.468462; 0.302684; 4.208285;,
        -1.314101; 0.313185; 4.200961;,
        -1.468461; 0.302684; 4.649817;,
        -2.051001; 0.263054; 4.622179;,
        -2.051002; 0.263054; 4.235924;,
        -1.468462; 0.302684; 4.208285;;
        14;
        4;0;1;2;3;,
        4;4;5;6;7;,
        4;8;9;10;11;,
        4;12;13;14;15;,
        4;16;17;18;19;,
        4;20;21;22;23;,
        4;24;25;26;27;,
        4;28;29;30;31;,
        4;32;33;34;35;,
        4;36;37;38;39;,
        4;40;41;42;43;,
        4;44;45;46;47;,
        4;48;49;50;51;,
        4;52;53;54;55;;
        MeshNormals { //armL_001 Normals
          56;
          -1.000000; 0.000001; 0.000001;,
          -1.000000; 0.000001; 0.000001;,
          -1.000000; 0.000001; 0.000001;,
          -1.000000; 0.000001; 0.000001;,
           1.000000;-0.000001;-0.000001;,
           1.000000;-0.000001;-0.000001;,
           1.000000;-0.000001;-0.000001;,
           1.000000;-0.000001;-0.000001;,
          -0.047394; 0.000000;-0.998876;,
          -0.047394; 0.000000;-0.998876;,
          -0.047394; 0.000000;-0.998876;,
          -0.047394; 0.000000;-0.998876;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.047393; 0.000000;-0.998876;,
          -0.047393; 0.000000;-0.998876;,
          -0.047393; 0.000000;-0.998876;,
          -0.047393; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;;
          14;
          4;0;1;2;3;,
          4;4;5;6;7;,
          4;8;9;10;11;,
          4;12;13;14;15;,
          4;16;17;18;19;,
          4;20;21;22;23;,
          4;24;25;26;27;,
          4;28;29;30;31;,
          4;32;33;34;35;,
          4;36;37;38;39;,
          4;40;41;42;43;,
          4;44;45;46;47;,
          4;48;49;50;51;,
          4;52;53;54;55;;
        } //End of armL_001 Normals
        MeshMaterialList { //armL_001 Material List
          1;
          14;
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
          Material None {
             0.640000; 0.640000; 0.640000; 1.000000;;
             498.039216;
             0.800000; 0.800000; 0.800000;;
             0.000000; 0.000000; 0.000000;;
          }
        } //End of armL_001 Material List
        XSkinMeshHeader {
          2;
          6;
          2;
        }
        SkinWeights {
          "Armature_Bone_L_001";
          56;
          0,
          1,
          2,
          3,
          4,
          5,
          6,
          7,
          8,
          9,
          10,
          11,
          12,
          13,
          14,
          15,
          16,
          17,
          18,
          19,
          20,
          21,
          22,
          23,
          24,
          25,
          26,
          27,
          28,
          29,
          30,
          31,
          32,
          33,
          34,
          35,
          36,
          37,
          38,
          39,
          40,
          41,
          42,
          43,
          44,
          45,
          46,
          47,
          48,
          49,
          50,
          51,
          52,
          53,
          54,
          55;
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          1.000000,
          1.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          1.000000,
          1.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000,
          1.000000,
          1.000000,
          0.000000,
          1.000000,
          0.000000,
          0.000000,
          1.000000,
          0.000000,
          0.000000,
          0.000000,
          0.000000;
           0.000000,-0.864408, 0.000000, 0.000000,
           0.404646, 0.000000, 0.000000, 0.000000,
          -0.000000,-0.000000, 0.437145, 0.000000,
          -0.003787,-0.722543,-1.957814, 1.000000;;
        }  //End of Armature_Bone_L_001 Skin Weights
        SkinWeights {
          "Armature_Bone_L_002";
          28;
          0,
          1,
          2,
          3,
          24,
          25,
          26,
          27,
          28,
          31,
          32,
          33,
          34,
          35,
          36,
          39,
          40,
          41,
          42,
          43,
          44,
          47,
          49,
          50,
          52,
          53,
          54,
          55;
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000;
           0.000000,-0.864408, 0.000000, 0.000000,
           0.404646, 0.000000, 0.000000, 0.000000,
          -0.000000,-0.000000, 0.437145, 0.000000,
          -0.003787,-1.299891,-1.957814, 1.000000;;
        }  //End of Armature_Bone_L_002 Skin Weights
      } //End of armL_001 Mesh
    } //End of armL
    Frame armR {
      FrameTransformMatrix {
        -0.864408,-0.000001, 0.000000, 0.000000,
         0.000001,-0.404646, 0.000000, 0.000000,
         0.000000,-0.000000, 0.437146, 0.000000,
        -0.277238,-0.003787,-0.957814, 1.000000;;
      }
      Mesh { //armR_001 Mesh
        56;
        -2.051001; 0.263054; 4.622179;,
        -2.051002;-0.290787; 4.622179;,
        -2.051002;-0.290788; 4.235924;,
        -2.051002; 0.263054; 4.235924;,
        -0.756924;-0.378827; 4.174525;,
        -0.756924;-0.378826; 4.683576;,
        -0.756923; 0.351089; 4.683576;,
        -0.756924; 0.351089; 4.174525;,
        -1.314101;-0.340921; 4.200961;,
        -0.756924;-0.378827; 4.174525;,
        -0.756924; 0.351089; 4.174525;,
        -1.314101; 0.313185; 4.200961;,
        -1.314101;-0.340920; 4.657141;,
        -0.756924;-0.378826; 4.683576;,
        -0.756924;-0.378827; 4.174525;,
        -1.314101;-0.340921; 4.200961;,
        -1.314100; 0.313185; 4.657141;,
        -0.756923; 0.351089; 4.683576;,
        -0.756924;-0.378826; 4.683576;,
        -1.314101;-0.340920; 4.657141;,
        -0.756923; 0.351089; 4.683576;,
        -1.314100; 0.313185; 4.657141;,
        -1.314101; 0.313185; 4.200961;,
        -0.756924; 0.351089; 4.174525;,
        -2.051002;-0.290788; 4.235924;,
        -1.468462;-0.330419; 4.208285;,
        -1.468462; 0.302684; 4.208285;,
        -2.051002; 0.263054; 4.235924;,
        -1.468462;-0.330419; 4.208285;,
        -1.314101;-0.340921; 4.200961;,
        -1.314101; 0.313185; 4.200961;,
        -1.468462; 0.302684; 4.208285;,
        -2.051002;-0.290787; 4.622179;,
        -1.468462;-0.330419; 4.649817;,
        -1.468462;-0.330419; 4.208285;,
        -2.051002;-0.290788; 4.235924;,
        -1.468462;-0.330419; 4.649817;,
        -1.314101;-0.340920; 4.657141;,
        -1.314101;-0.340921; 4.200961;,
        -1.468462;-0.330419; 4.208285;,
        -2.051001; 0.263054; 4.622179;,
        -1.468461; 0.302684; 4.649817;,
        -1.468462;-0.330419; 4.649817;,
        -2.051002;-0.290787; 4.622179;,
        -1.468461; 0.302684; 4.649817;,
        -1.314100; 0.313185; 4.657141;,
        -1.314101;-0.340920; 4.657141;,
        -1.468462;-0.330419; 4.649817;,
        -1.314100; 0.313185; 4.657141;,
        -1.468461; 0.302684; 4.649817;,
        -1.468462; 0.302684; 4.208285;,
        -1.314101; 0.313185; 4.200961;,
        -1.468461; 0.302684; 4.649817;,
        -2.051001; 0.263054; 4.622179;,
        -2.051002; 0.263054; 4.235924;,
        -1.468462; 0.302684; 4.208285;;
        14;
        4;0;1;2;3;,
        4;4;5;6;7;,
        4;8;9;10;11;,
        4;12;13;14;15;,
        4;16;17;18;19;,
        4;20;21;22;23;,
        4;24;25;26;27;,
        4;28;29;30;31;,
        4;32;33;34;35;,
        4;36;37;38;39;,
        4;40;41;42;43;,
        4;44;45;46;47;,
        4;48;49;50;51;,
        4;52;53;54;55;;
        MeshNormals { //armR_001 Normals
          56;
          -1.000000; 0.000001; 0.000001;,
          -1.000000; 0.000001; 0.000001;,
          -1.000000; 0.000001; 0.000001;,
          -1.000000; 0.000001; 0.000001;,
           1.000000;-0.000001;-0.000001;,
           1.000000;-0.000001;-0.000001;,
           1.000000;-0.000001;-0.000001;,
           1.000000;-0.000001;-0.000001;,
          -0.047394; 0.000000;-0.998876;,
          -0.047394; 0.000000;-0.998876;,
          -0.047394; 0.000000;-0.998876;,
          -0.047394; 0.000000;-0.998876;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.047393; 0.000000;-0.998876;,
          -0.047393; 0.000000;-0.998876;,
          -0.047393; 0.000000;-0.998876;,
          -0.047393; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.047392; 0.000000;-0.998876;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.067875;-0.997694; 0.000002;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047391; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.047392; 0.000000; 0.998876;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;,
          -0.067872; 0.997694; 0.000000;;
          14;
          4;0;1;2;3;,
          4;4;5;6;7;,
          4;8;9;10;11;,
          4;12;13;14;15;,
          4;16;17;18;19;,
          4;20;21;22;23;,
          4;24;25;26;27;,
          4;28;29;30;31;,
          4;32;33;34;35;,
          4;36;37;38;39;,
          4;40;41;42;43;,
          4;44;45;46;47;,
          4;48;49;50;51;,
          4;52;53;54;55;;
        } //End of armR_001 Normals
        MeshMaterialList { //armR_001 Material List
          1;
          14;
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
          Material None {
             0.640000; 0.640000; 0.640000; 1.000000;;
             498.039216;
             0.800000; 0.800000; 0.800000;;
             0.000000; 0.000000; 0.000000;;
          }
        } //End of armR_001 Material List
        XSkinMeshHeader {
          2;
          6;
          2;
        }
        SkinWeights {
          "Armature_Bone_R_002";
          28;
          0,
          1,
          2,
          3,
          24,
          25,
          26,
          27,
          28,
          31,
          32,
          33,
          34,
          35,
          36,
          39,
          40,
          41,
          42,
          43,
          44,
          47,
          49,
          50,
          52,
          53,
          54,
          55;
          0.985949,
          0.970257,
          0.966044,
          0.961052,
          0.966044,
          1.000000,
          1.000000,
          0.961052,
          1.000000,
          1.000000,
          0.970257,
          1.000000,
          1.000000,
          0.966044,
          1.000000,
          1.000000,
          0.985949,
          1.000000,
          1.000000,
          0.970257,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          0.985949,
          0.961052,
          1.000000;
           0.000001,-0.864408, 0.000000, 0.000000,
           0.404646, 0.000000, 0.000000, 0.000000,
           0.000000,-0.000000, 0.437145, 0.000000,
           0.003787,-1.295146,-1.957814, 1.000000;;
        }  //End of Armature_Bone_R_002 Skin Weights
        SkinWeights {
          "Armature_Bone_R_001";
          56;
          0,
          1,
          2,
          3,
          4,
          5,
          6,
          7,
          8,
          9,
          10,
          11,
          12,
          13,
          14,
          15,
          16,
          17,
          18,
          19,
          20,
          21,
          22,
          23,
          24,
          25,
          26,
          27,
          28,
          29,
          30,
          31,
          32,
          33,
          34,
          35,
          36,
          37,
          38,
          39,
          40,
          41,
          42,
          43,
          44,
          45,
          46,
          47,
          48,
          49,
          50,
          51,
          52,
          53,
          54,
          55;
          0.014051,
          0.029743,
          0.033956,
          0.038948,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          0.033956,
          0.000000,
          0.000000,
          0.038948,
          0.000000,
          1.000000,
          1.000000,
          0.000000,
          0.029743,
          0.000000,
          0.000000,
          0.033956,
          0.000000,
          1.000000,
          1.000000,
          0.000000,
          0.014051,
          0.000000,
          0.000000,
          0.029743,
          0.000000,
          1.000000,
          1.000000,
          0.000000,
          1.000000,
          0.000000,
          0.000000,
          1.000000,
          0.000000,
          0.014051,
          0.038948,
          0.000000;
           0.000001,-0.864408, 0.000000, 0.000000,
           0.404646, 0.000000, 0.000000, 0.000000,
           0.000000,-0.000000, 0.437145, 0.000000,
           0.003787,-0.717798,-1.957814, 1.000000;;
        }  //End of Armature_Bone_R_001 Skin Weights
      } //End of armR_001 Mesh
    } //End of armR
    Frame handL {
      FrameTransformMatrix {
         0.161241, 0.000000, 0.000000, 0.000000,
         0.000000, 0.161302, 0.000000, 0.000000,
         0.000000,-0.000000, 0.161302, 0.000000,
        -1.624110,-0.003787, 0.977854, 1.000000;;
      }
      Mesh { //handL_001 Mesh
        24;
        -1.000000; 1.000000;-1.000000;,
        -1.000000;-1.000000;-1.000000;,
         1.000000;-1.000000;-1.000000;,
         1.000000; 1.000000;-1.000000;,
         0.999999;-1.000001; 1.000000;,
        -1.000000;-1.000000; 1.000000;,
        -1.000000; 1.000000; 1.000000;,
         1.000000; 0.999999; 1.000000;,
         1.000000;-1.000000;-1.000000;,
         0.999999;-1.000001; 1.000000;,
         1.000000; 0.999999; 1.000000;,
         1.000000; 1.000000;-1.000000;,
        -1.000000;-1.000000;-1.000000;,
        -1.000000;-1.000000; 1.000000;,
         0.999999;-1.000001; 1.000000;,
         1.000000;-1.000000;-1.000000;,
        -1.000000; 1.000000;-1.000000;,
        -1.000000; 1.000000; 1.000000;,
        -1.000000;-1.000000; 1.000000;,
        -1.000000;-1.000000;-1.000000;,
        -1.000000; 1.000000; 1.000000;,
        -1.000000; 1.000000;-1.000000;,
         1.000000; 1.000000;-1.000000;,
         1.000000; 0.999999; 1.000000;;
        6;
        4;0;1;2;3;,
        4;4;5;6;7;,
        4;8;9;10;11;,
        4;12;13;14;15;,
        4;16;17;18;19;,
        4;20;21;22;23;;
        MeshNormals { //handL_001 Normals
          24;
           0.000000; 0.000000;-1.000000;,
           0.000000; 0.000000;-1.000000;,
           0.000000; 0.000000;-1.000000;,
           0.000000; 0.000000;-1.000000;,
           0.000000;-0.000000; 1.000000;,
           0.000000;-0.000000; 1.000000;,
           0.000000;-0.000000; 1.000000;,
           0.000000;-0.000000; 1.000000;,
           1.000000;-0.000000; 0.000000;,
           1.000000;-0.000000; 0.000000;,
           1.000000;-0.000000; 0.000000;,
           1.000000;-0.000000; 0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
           0.000000; 1.000000; 0.000000;,
           0.000000; 1.000000; 0.000000;,
           0.000000; 1.000000; 0.000000;,
           0.000000; 1.000000; 0.000000;;
          6;
          4;0;1;2;3;,
          4;4;5;6;7;,
          4;8;9;10;11;,
          4;12;13;14;15;,
          4;16;17;18;19;,
          4;20;21;22;23;;
        } //End of handL_001 Normals
        MeshMaterialList { //handL_001 Material List
          1;
          6;
          0,
          0,
          0,
          0,
          0,
          0;;
          Material None {
             0.640000; 0.640000; 0.640000; 1.000000;;
             498.039216;
             0.800000; 0.800000; 0.800000;;
             0.000000; 0.000000; 0.000000;;
          }
        } //End of handL_001 Material List
        XSkinMeshHeader {
          1;
          3;
          1;
        }
        SkinWeights {
          "Armature_Bone_L_003";
          24;
          0,
          1,
          2,
          3,
          4,
          5,
          6,
          7,
          8,
          9,
          10,
          11,
          12,
          13,
          14,
          15,
          16,
          17,
          18,
          19,
          20,
          21,
          22,
          23;
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000;
           0.000000,-0.161241, 0.000000, 0.000000,
           0.161302, 0.000000, 0.000000, 0.000000,
          -0.000000,-0.000000, 0.161302, 0.000000,
          -0.003787, 0.136100,-0.022146, 1.000000;;
        }  //End of Armature_Bone_L_003 Skin Weights
      } //End of handL_001 Mesh
    } //End of handL
    Frame handR {
      FrameTransformMatrix {
         0.161241, 0.000000, 0.000000, 0.000000,
         0.000000, 0.161302, 0.000000, 0.000000,
         0.000000,-0.000000, 0.161302, 0.000000,
         1.623959,-0.003787, 0.977854, 1.000000;;
      }
      Mesh { //handR_001 Mesh
        24;
        -1.000000; 1.000000;-1.000000;,
        -1.000000;-1.000000;-1.000000;,
         1.000000;-1.000000;-1.000000;,
         1.000000; 1.000000;-1.000000;,
         0.999999;-1.000001; 1.000000;,
        -1.000000;-1.000000; 1.000000;,
        -1.000000; 1.000000; 1.000000;,
         1.000000; 0.999999; 1.000000;,
         1.000000;-1.000000;-1.000000;,
         0.999999;-1.000001; 1.000000;,
         1.000000; 0.999999; 1.000000;,
         1.000000; 1.000000;-1.000000;,
        -1.000000;-1.000000;-1.000000;,
        -1.000000;-1.000000; 1.000000;,
         0.999999;-1.000001; 1.000000;,
         1.000000;-1.000000;-1.000000;,
        -1.000000; 1.000000;-1.000000;,
        -1.000000; 1.000000; 1.000000;,
        -1.000000;-1.000000; 1.000000;,
        -1.000000;-1.000000;-1.000000;,
        -1.000000; 1.000000; 1.000000;,
        -1.000000; 1.000000;-1.000000;,
         1.000000; 1.000000;-1.000000;,
         1.000000; 0.999999; 1.000000;;
        6;
        4;0;1;2;3;,
        4;4;5;6;7;,
        4;8;9;10;11;,
        4;12;13;14;15;,
        4;16;17;18;19;,
        4;20;21;22;23;;
        MeshNormals { //handR_001 Normals
          24;
           0.000000; 0.000000;-1.000000;,
           0.000000; 0.000000;-1.000000;,
           0.000000; 0.000000;-1.000000;,
           0.000000; 0.000000;-1.000000;,
           0.000000;-0.000000; 1.000000;,
           0.000000;-0.000000; 1.000000;,
           0.000000;-0.000000; 1.000000;,
           0.000000;-0.000000; 1.000000;,
           1.000000;-0.000000; 0.000000;,
           1.000000;-0.000000; 0.000000;,
           1.000000;-0.000000; 0.000000;,
           1.000000;-0.000000; 0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -0.000000;-1.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
          -1.000000; 0.000000;-0.000000;,
           0.000000; 1.000000; 0.000000;,
           0.000000; 1.000000; 0.000000;,
           0.000000; 1.000000; 0.000000;,
           0.000000; 1.000000; 0.000000;;
          6;
          4;0;1;2;3;,
          4;4;5;6;7;,
          4;8;9;10;11;,
          4;12;13;14;15;,
          4;16;17;18;19;,
          4;20;21;22;23;;
        } //End of handR_001 Normals
        MeshMaterialList { //handR_001 Material List
          1;
          6;
          0,
          0,
          0,
          0,
          0,
          0;;
          Material None {
             0.640000; 0.640000; 0.640000; 1.000000;;
             498.039216;
             0.800000; 0.800000; 0.800000;;
             0.000000; 0.000000; 0.000000;;
          }
        } //End of handR_001 Material List
        XSkinMeshHeader {
          1;
          3;
          1;
        }
        SkinWeights {
          "Armature_Bone_R_003";
          24;
          0,
          1,
          2,
          3,
          4,
          5,
          6,
          7,
          8,
          9,
          10,
          11,
          12,
          13,
          14,
          15,
          16,
          17,
          18,
          19,
          20,
          21,
          22,
          23;
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000,
          1.000000;
           0.000000, 0.161241, 0.000000, 0.000000,
          -0.161302, 0.000000, 0.000000, 0.000000,
           0.000000,-0.000000, 0.161302, 0.000000,
           0.003787, 0.135949,-0.022146, 1.000000;;
        }  //End of Armature_Bone_R_003 Skin Weights
      } //End of handR_001 Mesh
    } //End of handR
  } //End of Armature
} //End of Root Frame
AnimTicksPerSecond {
  24;
}
AnimationSet {
  Animation {
    {Armature_Bone_L_001}
    AnimationKey { //Position
      2;
      1;
      0;3;    -0.627410, 0.724446, 0.696274;;;
    }
    AnimationKey { //Rotation
      0;
      1;
      0;4;    -0.839602,-0.196055,-0.500936,-0.075462;;;
    }
    AnimationKey { //Scale
      1;
      1;
      0;3;     1.000000, 1.000000, 1.000000;;;
    }
  }
  Animation {
    {Armature_Bone_L_002}
    AnimationKey { //Position
      2;
      1;
      0;3;    -0.000000, 0.577348, 0.000000;;;
    }
    AnimationKey { //Rotation
      0;
      1;
      0;4;    -0.900806,-0.292804, 0.047030,-0.317178;;;
    }
    AnimationKey { //Scale
      1;
      1;
      0;3;     1.000000, 1.000000, 1.000000;;;
    }
  }
  Animation {
    {Armature_Bone_L_003}
    AnimationKey { //Position
      2;
      1;
      0;3;     0.000000, 0.470102,-0.000000;;;
    }
    AnimationKey { //Rotation
      0;
      1;
      0;4;    -0.986026,-0.063003, 0.025933,-0.152020;;;
    }
    AnimationKey { //Scale
      1;
      1;
      0;3;     1.000000, 1.000000, 1.000000;;;
    }
  }
  Animation {
    {Armature_Bone_R_001}
    AnimationKey { //Position
      2;
      1;
      0;3;     0.190389,-0.266597, 0.906023;;;
    }
    AnimationKey { //Rotation
      0;
      1;
      0;4;    -0.810503,-0.181138, 0.372702,-0.413966;;;
    }
    AnimationKey { //Scale
      1;
      1;
      0;3;     1.000000, 1.000000, 1.000000;;;
    }
  }
  Animation {
    {Armature_Bone_R_002}
    AnimationKey { //Position
      2;
      1;
      0;3;    -0.000000, 0.577348,-0.000000;;;
    }
    AnimationKey { //Rotation
      0;
      1;
      0;4;    -0.871421,-0.104451, 0.052235, 0.476432;;;
    }
    AnimationKey { //Scale
      1;
      1;
      0;3;     1.000000, 1.000000, 1.000000;;;
    }
  }
  Animation {
    {Armature_Bone_R_003}
    AnimationKey { //Position
      2;
      1;
      0;3;     0.000000, 0.470102,-0.000000;;;
    }
    AnimationKey { //Rotation
      0;
      1;
      0;4;    -0.978047,-0.076927, 0.139947, 0.133868;;;
    }
    AnimationKey { //Scale
      1;
      1;
      0;3;     1.000000, 1.000000, 1.000000;;;
    }
  }
} //End of AnimationSet

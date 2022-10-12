#pragma once

namespace nsK2EngineLow {

    class RenderingEngine;
    /// <summary>
    /// フォワードレンダリング用のモデル初期化構造体。
    /// </summary>
    /// <remark>
    /// ModelInitDataを継承しています。
    /// フォワードレンダリングのために必要なデフォルト設定をコンストラクタで行ってくれます。
    /// </remark>
    struct ModelInitDataFR : public ModelInitData
    {
        ModelInitDataFR()
        {
            m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
    };
    class ModelRender
    {
    public:

        /// <summary>
        /// 初期化
        /// </summary>
        /// <remark>
        /// 本関数を利用して初期化されたモデルは、
        /// ディファードレンダリングの描画パスで描画されます
        /// ディファードレンダリングでの描画はPBRシェーダーでライティングが計算されます
        /// </remark>
        /// <param name="renderingEngine">レンダリングエンジン。</param>
        /// <param name="tkmFilePath">tkmファイルパス。</param>
        /// <param name="isShadowReciever">シャドウレシーバーフラグ</param>
        void InitDeferredRendering(RenderingEngine& renderingEngine, const char* tkmFilePath, bool isShadowReciever);

        /// <summary>
        /// 初期化
        /// </summary>
        /// <remark>
        /// 本関数を利用して初期化されたモデルは
        /// フォワードレンダリングの描画パスで描画されます
        /// 特殊なシェーディングを行いたい場合は、こちらを使用してください
        /// </remark>
        /// <param name="renderingEngine">レンダリングエンジン</param>
        /// <param name="modelInitData"></param>
        void InitForwardRendering(RenderingEngine& renderingEngine, ModelInitDataFR& modelInitData);

        /// <summary>
        /// ワールド行列を更新する
        /// </summary>
        /// <param name="pos">座標</param>
        /// <param name="rot">回転クォータニオン</param>
        /// <param name="scale">拡大率</param>
        void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
        {
 //           m_zprepassModel.UpdateWorldMatrix(pos, rot, scale);
            if (m_renderToGBufferModel.IsInited())
            {
                m_renderToGBufferModel.UpdateWorldMatrix(pos, rot, scale);
            }
            if (m_forwardRenderModel.IsInited())
            {
                m_forwardRenderModel.UpdateWorldMatrix(pos, rot, scale);
            }
        }

        /// <summary>
        /// 描画
        /// </summary>
        void Draw();

        /// <summary>
        /// シャドウキャスターのフラグを設定する
        /// </summary>
        void SetShadowCasterFlag(bool flag)
        {
            m_isShadowCaster = flag;
        }



        TkmFile* GetTkm()
        {
            return m_renderToGBufferModel.GetTkmFile();
        }

        //ベクトル内積
        double dot_product(const Vector3& vl, const Vector3 vr) {
            return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
        }

        //三角形と点の当たり判定
        int HitTestPolygon(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
        {
            Vector3 AB, BP, BC, CP, CA, AP;

            //ベクトルの乗算
            //V1(AB),V2(BC),V3(CA)を計算。
            //P0H(BP),P1H(CP),P2H(AP)を計算
            AB.Subtract(B, A);
            BP.Subtract(P, B);

            BC.Subtract(C, B);
            CP.Subtract(P, C);

            CA.Subtract(A, C);
            AP.Subtract(P, A);

            Vector3 c1, c2, c3;

            //外積を求める。
            //V1P0H(AB*BP),V2P1H(BC*CP),V3P2H(CA*AP)を計算
            c1.Cross(AB, BP);
            c2.Cross(BC, CP);
            c3.Cross(CA, AP);

            //内積で順方向か逆方向か調べる
            double dot_12 = dot_product(c1, c2);
            double dot_13 = dot_product(c1, c3);

            if (dot_12 > 0 && dot_13 > 0) {
                //三角形の内側に点がある
                return 0;
            }

            //三角形の外側に点がある
            return 1;
        }


        //線分ABと平面の交点を計算する
        bool IntersectPlaneAndLine(
            Vector3& out, //戻り値　交点が見つかれば格納される
            Vector3  A,   //線分始点
            Vector3  B,   //線分終点
            std::vector<nsK2EngineLow::TkmFile::VectorBuffer> vectorBuffer
        ) //平面
        {
            for (int i = 0;i < vectorBuffer.size();i++)
            {

                Matrix matrix;

                matrix = m_renderToGBufferModel.GetWorldMatrix();

                matrix.Apply(vectorBuffer[i].buffer[0]);
                matrix.Apply(vectorBuffer[i].buffer[1]);
                matrix.Apply(vectorBuffer[i].buffer[2]);



                //平面上の点P
                //点P１＝頂点１（Vector3）
                Vector3 P = Vector3(vectorBuffer[i].buffer[0]);

                //PA PBベクトル
                Vector3 PA = Vector3(P.x - A.x, P.y - A.y, P.z - A.z);
                Vector3 PB = Vector3(P.x - B.x, P.y - B.y, P.z - B.z);

                //PA PBそれぞれを平面法線と内積
                double dot_PA = PA.Dot(vectorBuffer[i].normal);
                double dot_PB = PB.Dot(vectorBuffer[i].normal);

                //これは線端が平面上にあった時の計算の誤差を吸収しています。調整して使ってください。
                if (abs(dot_PA) < 0.000001) { dot_PA = 0.0; }
                if (abs(dot_PB) < 0.000001) { dot_PB = 0.0; }

                //交差判定
                if (dot_PA == 0.0 && dot_PB == 0.0) {
                    //両端が平面上にあり、交点を計算できない。
                    continue;
                }
                if ((dot_PA >= 0.0 && dot_PB <= 0.0) ||
                    (dot_PA <= 0.0 && dot_PB >= 0.0)) {
                    //内積の片方がプラスで片方がマイナスなので、交差している

                }
                else {
                    //交差していない
                    continue;
                }

                //以下、交点を求める 

                Vector3 AB = Vector3(B.x - A.x, B.y - A.y, B.z - A.z);

                //交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB
                double hiritu = abs(dot_PA) / (abs(dot_PA) + abs(dot_PB));

                out.x = A.x + (AB.x * hiritu);
                out.y = A.y + (AB.y * hiritu);
                out.z = A.z + (AB.z * hiritu);


                if (HitTestPolygon(vectorBuffer[i].buffer[0], vectorBuffer[i].buffer[1], vectorBuffer[i].buffer[2], out) == 1)
                {
                    continue;
                }

                return true;

            }
            return false;
        }





    private:
        /// <summary>
        /// 共通の初期化処理
        /// </summary>
        /// <param name="renderingEngine">レンダリングエンジン</param>
        /// <param name="tkmFilePath">tkmファイルパス</param>
        void InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath);

    private:
        RenderingEngine* m_renderingEngine = nullptr;   //レンダリングエンジン
        Model m_zprepassModel;                  // ZPrepassで描画されるモデル
        Model m_forwardRenderModel;             // フォワードレンダリングの描画パスで描画されるモデル
        Model m_renderToGBufferModel;           // RenderToGBufferで描画されるモデル
        Model m_shadowModels[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];	//シャドウマップに描画するモデル
        bool m_isShadowCaster = false;          // シャドウキャスターフラグ
    };
}
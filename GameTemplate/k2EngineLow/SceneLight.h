#pragma once

namespace nsK2EngineLow {

    /// <summary>
   /// ライト構造体。
   /// </summary>
    struct Directionlight {
        Vector3 dirDirection;	//ライトの方向。
        float pad;
        Vector3 dirColor;
        float pad1;
        int castShadow;     // 影をキャストする？

    public:
        const Vector3& GetDirection()const
        {
            return dirDirection;
        }
    };
    struct Pointlight {
        Vector3 ptPosition;		//位置。
        float pad;				//パディング。
        Vector3 ptColor;		//カラー。
        float ptRange;			//影響範囲。
    };
    struct Spotlight {
        Vector3 spPosition;		//位置。
        float pad;				//パディング。
        Vector3 spColor;		//カラー。
        float spRange;			//影響範囲。
        Vector3 spDirection;	//射出方向。
        float spAngle;			//射出角度。
    };

    struct Light {

        Directionlight directionlight; //ディレクションライト。
        Pointlight pointlight; //ポイントライト。
        Spotlight spotlight; //スポットライト。

        Vector3 ambientLight;
        float pad;
        Vector3 eyePos;
        float pad2;
        float specPow;          // スペキュラの絞り
    };

    class SceneLight
    {
    public:
        SceneLight() {}
        ~SceneLight();
  
        void Init();
        void Update();

        /// <summary>
        /// ディレクションライトの方向を設定する。
        /// </summary>
        /// <param name="light">方向</param>
        void SetLigDirection(const Vector3& light)
        {
            m_light.directionlight.dirDirection = light;
        }
        /// <summary>
        /// ディレクションライトのカラーを設定する。
        /// </summary>
        /// <param name="light">カラー</param>
        void SetLigColor(const Vector3& color)
        {
            m_light.directionlight.dirColor = color;
        }

        /// <summary>
            /// 環境光を設定する。
            /// </summary>
            /// <param name="col">カラー。</param>
        void SetAmbientLight(const Vector3& col)
        {
            m_light.ambientLight = col;
        }


        /// <summary>
         /// ポイントライトの場所を設定する。
         /// </summary>
         /// <param name="light">場所</param>
        void SetLigPoint(const Vector3& lightposi)
        {
            m_light.pointlight.ptPosition = lightposi;
        }
        /// <summary>
         /// ポイントライトのカラーを設定する。
         /// </summary>
         /// <param name="light">カラー</param>
        void SetLigPointColor(const Vector3& lightcola)
        {
            m_light.pointlight.ptColor = lightcola;
        }
        /// <summary>
        /// ポイントライトの影響範囲を設定。
        /// </summary>
        /// <param name="range"></param>
        void SetPointRange(float range)
        {
            m_light.pointlight.ptRange = range;
        }


        /// <summary>
         /// スポットライトの回転を設定する。
         /// </summary>
         /// <param name="rot">回転</param>
        void SetLigSpotRot(const Vector3& lightrot)
        {
            m_light.spotlight.spDirection = lightrot;
        }
        /// <summary>
        /// スポットライトの場所を設定する。
        /// </summary>
        /// <param name="posi">場所</param>
        void SetLigSpot(const Vector3& posi)
        {
            m_light.spotlight.spPosition = posi;
        }
        /// <summary>
        /// スポットライトの色を設定する。
        /// </summary>
        /// <param name="Color">色</param>
        void SetLigSpotColor(const Vector3& color)
        {
            m_light.spotlight.spColor = color;
        }


        /// <summary>
        /// 視点のポジションを設定する。
        /// </summary>
        /// <param name="pos">カメラのポジション。</param>
        void SeteyePosition(const Vector3& pos)
        {
            m_light.eyePos = pos;
        }

        /// <summary>
        /// ディレクションライトの方向を取得する
        /// </summary>
        /// <returns>方向</returns>
        const Vector3& GetDirectionLightDirection()const
        {
            return m_light.directionlight.GetDirection();
        }

        /// <summary>
        /// ライトの情報を取得する
        /// </summary>
        /// <returns>ライト</returns>
        Light& GetLight()
        {
            return m_light;
        }

        /// <summary>
        /// 環境光のカラーを取得する
        /// </summary>
        /// <returns>カラー</returns>
        const Vector3& GetAmbientLight()
        {
            return m_light.ambientLight;
        }


    private:
        Light    m_light;  //シーンライト。
        //float posi = 0.0f;
    };
    //グローバル変数。 (extern)
    extern SceneLight g_Light;
};


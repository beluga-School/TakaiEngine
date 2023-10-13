# TakaiEngine

DirectX12
WindowsSDK:10.0.22000.0

## 基本機能  
- directx12を用いた3D描画  
- .objの読み込み
- 自作のBlenderアドオンを用いたシーンエディタから出力された.jsonを読み込み、ゲーム内のオブジェクトを配置する機能
  - プレイヤーの初期位置を変更  
  - 地形、敵、アイテム、ゴール、ステージギミックの配置  
  - 当たり判定の読み込み
  - イベントの配置
  - イベント時のカメラの配置
  
- モデルを法線方向に拡大する方法を用いたアウトライン描画
- DInputとXInputを使用したキーボード、マウス、ゲームパッド入力
- XAudio2を使用したサウンド再生

## 使用した外部ライブラリ
- Assimp
- DirectxTex
- Dear ImGui

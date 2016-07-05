#ifndef _IMAGE_GO_2D_H_
#define _IMAGE_GO_2D_H_
#include "GameObject2D.h"
#include "DrawData2D.h"
#include "GameData.h"
#include "RenderTarget.h"

class ImageGO2D :public GameObject2D
{
public:
	ImageGO2D(string _fileName, ID3D11Device* _GD, PASS _pass);
	ImageGO2D(ID3D11ShaderResourceView* _pTextureRV, PASS _pass);
	virtual ~ImageGO2D();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);
protected:
	PASS m_pass;
	ID3D11ShaderResourceView* m_pTextureRV;
};

#endif
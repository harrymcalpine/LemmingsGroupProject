#pragma once
#include <windows.h>

#include <d3d11.h>
#include <directxmath.h>
#include <iostream>

using namespace DirectX;

inline XMVECTOR GMathFV(XMFLOAT3& val)
{
	return XMLoadFloat3(&val);
}

inline XMFLOAT3 GMathVF(XMVECTOR& vec)
{
	XMFLOAT3 val;
	XMStoreFloat3(&val, vec);
	return val;
}
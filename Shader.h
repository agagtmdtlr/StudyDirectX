#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <unordered_map>
#include <bitset>
#include <d3dcommon.h>
#include <array>
#include <d3dcompiler.h>

#include "PrimitiveBuffer.h"
#include "ConstantBuffer.h"
#include "set"

enum class RenderStage : UINT
{
	VS = 0,
	PS = VS + 1,
	GS = PS + 1,
	CS = GS + 1,
	NUM = CS + 1,
	Begin = 0
};


#define StageCount (static_cast<size_t>(RenderStage::NUM))

using BindStageMask = std::bitset<static_cast<size_t>(RenderStage::NUM)>;

struct RenderPassState
{
	void SetFlag(RenderStage flag, bool val)
	{
		mask[static_cast<size_t>(flag)] = val;
	}

	bool CheckMask(RenderStage flag) const
	{
		return mask[static_cast<size_t>(flag)];
	}

	Vector4 clearColor;
	BindStageMask mask;
};

struct Shader
{
	struct BindDesc
	{
		BindDesc() { ZeroMemory(&desc,sizeof(desc)); }
		BindDesc(D3D11_SHADER_INPUT_BIND_DESC desc, BindStageMask mask)	: desc(desc), mask(mask)
		{}
		D3D11_SHADER_INPUT_BIND_DESC desc;
		BindStageMask mask;
	};

	Shader(std::wstring shaderName, RenderPassState state);

	D3D11_VIEWPORT viewport;

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11GeometryShader> geometryShader;
	ComPtr<ID3D11DomainShader> domainShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	ComPtr<ID3D11ComputeShader> computeShader;
	ComPtr<ID3D11InputLayout> inputlayout;

	std::array<ComPtr<ID3DBlob>, StageCount> blobs;

	RenderPassState state;

	D3D11_SHADER_DESC shaderDesc;

	std::unordered_map<std::string, BindDesc> bindDescMap;
	std::unordered_map<std::string, ConstantBuffer> constantBuffers;

	bool UpdateConstantData(std::string name, Update_ConstantBuffe_Desc desc);

	bool BindSRV(std::string name, ID3D11ShaderResourceView* resource);
	void BindRenderTargets(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
	void BindDepthStencilState(ID3D11DepthStencilState* state);
	bool BindSampler(std::string name, ID3D11SamplerState* resource);


	void Initialize(std::wstring shaderName, RenderPassState state);
	void InitializeInputLayout(D3D11_SHADER_DESC shaderDesc, ID3D11ShaderReflection* reflection, ID3DBlob* blob);

	void DrawIndexed(UINT indexCount, UINT StartIndexLocation, INT BseVertexLocation);

	void BeginDraw(ID3D11RenderTargetView* rtv);
	void EndDraw();

	void BindPrimitiveBuffer(PrimitiveBuffer* buffer);

private:
	void BindConstantBufferToStage(std::string name, const ConstantBuffer& cbuffer);

	ID3DBlob* GetBlob(RenderStage stage);
	ID3DBlob** GetBlobAddressOf(RenderStage stage);

	/*
	set sample
	set shader resource ( csv , srv, uav
	*/
	

};


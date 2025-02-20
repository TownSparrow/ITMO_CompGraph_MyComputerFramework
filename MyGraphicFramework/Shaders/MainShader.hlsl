struct VS_IN
{
  float4 pos : POSITION0;
  float4 col : COLOR0;
};

struct PS_IN
{
  float4 pos : SV_POSITION;
  float4 col : COLOR;
};

PS_IN VSMain(VS_IN input)
{
  PS_IN output;
  output.pos = input.pos;
  output.col = input.col;
  return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
  float4 col = input.col;
#ifdef TEST
    if (input.pos.x > 400)
        col = float4(0.0f, 1.0f, 0.0f, 1.0f);
#endif
  return col;
}
#include "1019cc.h"

char *user_input;

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    error("引数の個数が正しくありません");
    return 1;
  }
  // トークナイズしてパースする
  user_input = argv[1];
  token = tokenize();
  Node *node = program();

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");
  
  // プロローグ
  // 変数26個分の領域を確保する
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  for (Node *n = node; n; n = n->next)
    gen(n);

  printf(".L.return:\n");
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");

  return 0;
}
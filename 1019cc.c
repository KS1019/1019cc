#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの型
enum {
	TK_NUM = 256, // 整数を表すトークン
	TK_EOF, // ファイルの終わりを示すトークン
};

enum {
	ND_NUM = 256 //整数のノードの型
};

typedef struct {
	int ty;	// トークンの型
	int val; // 値（数値用）
	char *input; // エラーメッセージ
} Token;

typedef struct Node {
  int ty;           // 演算子かND_NUM
  struct Node *lhs; // 左辺
  struct Node *rhs; // 右辺
  int val;          // tyがND_NUMの場合のみ使う
} Node;

Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

// 入力プログラム
char *user_input;

Token tokens[100];

// printfと同じ引数を取る
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// エラー箇所を報告するための関数
void error_at(char *loc, char *msg) {
  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, ""); // pos個の空白を出力
  fprintf(stderr, "^ %s\n", msg);
  exit(1);
}

// user_inputが指している文字列を
// トークンに分割してtokensに保存する
void tokenize() {
  char *p = user_input;

  int i = 0;
  while (*p) {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "引数の個数が正しくありません\n");
		return 1;
	}

		// トークナイズする
	user_input = argv[1];
	tokenize();

	// アセンブリの前半部分を出力
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	// 式の最初は数でなければならないので、それをチェックして
	// 最初のmov命令を出力
	if (tokens[0].ty != TK_NUM)
		error_at(tokens[0].input, "数ではありません");
	printf("  mov rax, %d\n", tokens[0].val);

	// `+ <数>`あるいは`- <数>`というトークンの並びを消費しつつ
	// アセンブリを出力
	int i = 1;
	while (tokens[i].ty != TK_EOF) {
		if (tokens[i].ty == '+') {
		i++;
		if (tokens[i].ty != TK_NUM)
			error_at(tokens[i].input, "数ではありません");
		printf("  add rax, %d\n", tokens[i].val);
		i++;
		continue;
		}

		if (tokens[i].ty == '-') {
		i++;
		if (tokens[i].ty != TK_NUM)
			error_at(tokens[i].input, "数ではありません");
		printf("  sub rax, %d\n", tokens[i].val);
		i++;
		continue;
		}

		error_at(tokens[i].input, "予期しないトークンです");
	}

	printf("  ret\n");
	return 0;
}

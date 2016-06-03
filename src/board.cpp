
#include "common.h"
#include "board.h"
#include "utility.h"

shared_ptr<Board> Board::current_ = std::make_shared<Board>();

Board::Board()
	: Board("startpos", "w", "-")
{
}

Board::Board(string sfen_board, string sfen_turn, string sfen_captured)
{
	if ( sfen_board == "startpos" ) {
		sfen_board = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL";
	}

	memset(piece_pos_, 0, sizeof(piece_pos_));

	// SFEN文字列の盤上情報を、バイトボードに変換
	uint8 byte_board[9][9] = { 0, };
	int i = 0, j = 0;  // i:筋, j:段
	for ( auto c : sfen_board ) {
		switch ( c ) {
			// 歩兵
			case 'P': byte_board[i][j] = 0x80;
			case 'p': byte_board[i][j] += PieceType_Pawn + 1; break;
			// 香車
			case 'L': byte_board[i][j] = 0x80;
			case 'l': byte_board[i][j] += PieceType_Lance + 1; break;
			// 桂馬
			case 'N': byte_board[i][j] = 0x80;
			case 'n': byte_board[i][j] += PieceType_Knight + 1; break;
			// 銀将
			case 'S': byte_board[i][j] = 0x80;
			case 's': byte_board[i][j] += PieceType_Silver + 1; break;
			// 角行
			case 'B': byte_board[i][j] = 0x80;
			case 'b': byte_board[i][j] += PieceType_Bishop + 1; break;
			// 飛車
			case 'R': byte_board[i][j] = 0x80;
			case 'r': byte_board[i][j] += PieceType_Rook + 1; break;
			// 金将
			case 'G': byte_board[i][j] = 0x80;
			case 'g': byte_board[i][j] += PieceType_Gold + 1; break;
			// 金将
			case 'K': byte_board[i][j] = 0x80;
			case 'k': byte_board[i][j] += PieceType_King + 1; break;
			// 成
			case '+': byte_board[i][j] += PieceType_Pro; break;
			// 段区切り
			case '/': i = -1; j++; break;
			//
			default:
				// 無駒数
				if ( std::isdigit(c) ) { i += c - '0' - 1; }
				break;
		}
		i++;
	}

	// SFEN文字列の持駒情報を持駒ビットボードに変換
	if ( !sfen_captured.empty() && sfen_captured != "-" ) {
		for ( auto c : sfen_board ) {
			switch ( c ) {
				// 歩兵
				case 'P': case 'p': ; break;
				// 香車
				case 'L': case 'l': ; break;
				// 桂馬
				case 'N': case 'n': ; break;
				// 銀将
				case 'S': case 's': ; break;
				// 角行
				case 'B': case 'b': ; break;
				// 飛車
				case 'R': case 'r': ; break;
				// 金将
				case 'G': case 'g': ; break;
				// 金将
				case 'K': case 'k': ; break;
			}
		}
	}

	// バイトボードをビットボードに変換
	for ( int i = 0; i < 9; i++ ) {
		for ( int j = 0; j < 9; j++ ) {
			if ( byte_board[i][j] != 0 ) {
				auto turn = byte_board[i][j] < 'a' ? 0 : 1;
				auto piece = (byte_board[i][j] & 0x7F) - 1;
				piece_pos_[turn][piece][i < 5 ? 0 : 1] |= 1ull << ((i < 5 ? i : i - 5) * 9 + j);
			}
		}
	}
}

ostream& operator << (ostream& os, const Board& board)
{
	uint8 byte_board[9][9] = { 0, };
	for ( int i = 0; i < 9; i++ ) {
		for ( int j = 0; j < 9; j++ ) {
			for ( int k = PieceType_Begin; k < PieceType_End; k++ ) {
				if ( i < 5 ? board.piece_pos_[0][k][0] & (1ull << (i * 9 + j)) : board.piece_pos_[0][k][1] & (1ull << ((i - 5) * 9 + j)) ) {
					byte_board[i][j] = k + 0x01;
					break;
				}
				if ( i < 5 ? board.piece_pos_[1][k][0] & (1ull << (i * 9 + j)) : board.piece_pos_[1][k][1] & (1ull << ((i - 5) * 9 + j)) ) {
					byte_board[i][j] = k + 0x81;
					break;
				}
			}
		}
	}

	os << "    9   8   7   6   5   4   3   2   1  \n";
	os << "  +---+---+---+---+---+---+---+---+---+\n";
	for ( int j = 0; j < 9; j++ ) {
		os << (j + 1) << " |";
		for ( int i = 0; i < 9; i++ ) {
			auto piece = byte_board[i][j] - 1;
			if ( piece != -1 ) {
				os << (!(piece & 0x80) ? "^" : "");
				switch ( piece & 0x7F ) {
					case PieceType_Pawn:   os << ((piece & 0x7F) < PieceType_Pro ? "歩" : "と"); break;
					case PieceType_Lance:  os << ((piece & 0x7F) < PieceType_Pro ? "香" : "杏"); break;
					case PieceType_Knight: os << ((piece & 0x7F) < PieceType_Pro ? "桂" : "圭"); break;
					case PieceType_Silver: os << ((piece & 0x7F) < PieceType_Pro ? "銀" : "全"); break;
					case PieceType_Bishop: os << ((piece & 0x7F) < PieceType_Pro ? "角" : "馬"); break;
					case PieceType_Rook:   os << ((piece & 0x7F) < PieceType_Pro ? "飛" : "竜"); break;
					case PieceType_Gold:   os << "金"; break;
					case PieceType_King:   os << "玉"; break;
				}
				os << ((byte_board[i][j] & 0x80) ? "." : "");
			}
			else {
				os << "   ";
			}
			os << "|";
		}
		os << "\n";
		os << "  +---+---+---+---+---+---+---+---+---+\n";
	}

	os << "手駒 : 先(";
	os << ") 後(";
	os << ")\n";

	os << std::endl;

	return os;
}

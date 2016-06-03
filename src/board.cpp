
#include "common.h"
#include "board.h"
#include "utility.h"

shared_ptr<Board> Board::current_;

Board::Board()
	: Board("startpos")
{
}

Board::Board(string sfen)
{
	if ( sfen == "startpos" ) {
		sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL";
	}

	memset(piece_pos_, 0, sizeof(piece_pos_));

	// SFEN������̋�����A�o�C�g�{�[�h�ɕϊ�
	uint8 byte_board[9][9] = { 0, };
	int i = 0, j = 0;  // i:��, j:�i
	for ( auto c : sfen ) {
		switch ( c ) {
			// ����
			case 'P': case 'p': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_Pawn + 1; break;
			// ����
			case 'L': case 'l': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_Lance + 1; break;
			// �j�n
			case 'N': case 'n': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_Knight + 1; break;
			// �⏫
			case 'S': case 's': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_Silver + 1; break;
			// �p�s
			case 'B': case 'b': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_Bishop + 1; break;
			// ���
			case 'R': case 'r': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_Rook + 1; break;
			// ����
			case 'G': case 'g': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_Gold + 1; break;
			// ����
			case 'K': case 'k': byte_board[i][j] = (c < 'a' ? 0 : 0x80) + PieceType_King + 1; break;
			// ��
			case '+': byte_board[i][j - 1] += PieceType_Pro; break;
			// �i��؂�
			case '/': i = -1; j++; break;
			//
			default:
				// ���
				if ( std::isdigit(c) ) { i += c - '0' - 1; }
				break;
		}
		i++;
	}

	// �o�C�g�{�[�h���r�b�g�{�[�h�ɕϊ�
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
					case PieceType_Pawn:   os << ((piece & 0x7F) < PieceType_Pro ? "��" : "��"); break;
					case PieceType_Lance:  os << ((piece & 0x7F) < PieceType_Pro ? "��" : "��"); break;
					case PieceType_Knight: os << ((piece & 0x7F) < PieceType_Pro ? "�j" : "�\"); break;
					case PieceType_Silver: os << ((piece & 0x7F) < PieceType_Pro ? "��" : "�S"); break;
					case PieceType_Bishop: os << ((piece & 0x7F) < PieceType_Pro ? "�p" : "�n"); break;
					case PieceType_Rook:   os << ((piece & 0x7F) < PieceType_Pro ? "��" : "��"); break;
					case PieceType_Gold:   os << "��"; break;
					case PieceType_King:   os << "��"; break;
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

	os << "��� : ��(";
	os << ") ��(";
	os << ")\n";

	os << std::endl;

	return os;
}

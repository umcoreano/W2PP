/*
*   Copyright (C) {2015}  {VK, Charles TheHouse}
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see [http://www.gnu.org/licenses/].
*
*/
#include "ProcessClientMessage.h"

void Exec_MSG_Withdraw(int conn, char *pMsg)
{
	MSG_STANDARDPARM *m = (MSG_STANDARDPARM*)pMsg;

	if (pMob[conn].MOB.CurrentScore.Hp == 0 || pUser[conn].Mode != USER_PLAY)
	{
		SendHpMode(conn);
		AddCrackError(conn, 10, 2);
		return;
	}

	int coin = m->Parm;

	if (pUser[conn].Coin >= coin && coin >= 0 && coin <= 2000000000)
	{
		int tcoin = coin + pMob[conn].MOB.Coin;

		if (tcoin >= 0 && tcoin <= 2000000000)
		{
			pMob[conn].MOB.Coin = tcoin;
			pUser[conn].Coin -= coin;

			m->ID = 30000;

			pUser[conn].cSock.AddMessage((char*)m, sizeof(MSG_STANDARDPARM));
			SendCargoCoin(conn);

			sprintf(temp, "etc,retirada do bau V:%d R:%d", coin, pUser[conn].Coin);
			Log(temp, pUser[conn].AccountName, pUser[conn].IP);
		}
		else
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_get_more_than_2G]);
	}
	else
		SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Withdraw_That_Much]);
}

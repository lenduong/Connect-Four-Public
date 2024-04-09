//----------- Additional function for AI ----------------------------
bool connect2Plus1(
  BoardValue** board, 
  BoardValue testPlayer/*player that's being checked for connect X*/,
  BoardValue currentPlayer,
  int ydim, int xdim, 
  int sy, int sx,
  int* y, int* x)
{
  // *********** Function Description**********
  // This function checks for connect 2, space, then another piece. We will refer
  //  to this pattern as the winning pattern in this function
  // Example: R R _ R 
  // Also checks for 1piece, space, and a connect 2
  // Example: R _ R R
  // The previous function wouldn't be able to recognize this to block/ play it
  // This function will be very similar to checkConnectX
  // NOTE: This function doesn't work as well as it should and requires some tweaking
  //  however, it works most of the time. 

  const int NDIRS = 4;
  const int xDirDelta[NDIRS] = { 0, +1, -1, +1};
  const int yDirDelta[NDIRS] = {+1,  0, +1, +1};
    
  // We go through every axis until there's a connect X
  //  and a piece can be placed at the start or end
  for (int axis=0; axis<4; axis++){
    bool flag = 0;
    // Check vert, hor, diags 
    // Axis 0: vertical
    // Axis 1: horizontal
    // Axis 2: diagonal (top left to bottom right)
    // Axis 3: diagonal (bottom left to top right)

    // We'll check in only 1 direction because this function will be used on every piece
    // So regardless of the direction we'll be able to find the pattern
    // In this direction, we will keep adding xDirDelta and yDirDelta

    // The row and col varibles below just have to be mutiplied to the same number to jump to 
    //  that many pieces away (in the same axis)
    int row = yDirDelta[axis];
    int col = xDirDelta[axis];

    // Check if the next 3 connected rows and columns are within range
    // Otherwise it'll give segmentation fault
    // If not within range then return false and AI will move on
    for (int check=1; check<4; check++){
      if ((sx+check*col)<0 || (sx+check*col)>=xdim || (sy+check*row)<0 || (sy+check*row)>=ydim)
      {flag = true; break;}
    }
    if (flag) {continue;}

    // Start searching for the winning pattern
    if (board[sy+row][sx+col] == testPlayer && board[sy+2*row][sx+2*col] == BLANK && 
    board[sy+3*row][sx+3*col] == testPlayer && findYValue(board, ydim, (sx+2*col)) == (sy+2*row)){
      // The last parameter checks for if there's an BLANK below the intended coordinate
      //  if there is then there's no potential to win or lose
      // Pattern: R R _ R
      // If true, set the current player's piece where the blank is
      board[sy+2*row][sx+2*col] = currentPlayer;
      *x = sx+2*col;
      *y = sy+2*row;
      return true;
    } 
    else if (board[sy+row][sx+col] == BLANK && board[sy+2*row][sx+2*col] == testPlayer 
    && board[sy+3*row][sx+3*col] == testPlayer && findYValue(board, ydim, (sx+col)) == (sy+row)) {
      // Pattern: R _ R R 
      board[sy+row][sx+col] = currentPlayer;
      *x = sx+col;
      *y = sy+row;
      return true;
    }
  }
  // If code reaches here, there's no winning pattern
  return false;
}



bool getUserAIInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  BoardValue player = playerToValue[currentPlayer];
  BoardValue opponent = playerToValue[1-currentPlayer];
  // Check if the AI can win
  // AI will first try to win by checking if it has a "R _ R R" or
  //  "R R _ R" pattern 
  // If it can win, it'll play its piece in the BLANK and win the game
  for (int row=0; row<ydim; row++){
    for (int column=0; column<xdim; column++){
      // If blank, do nothing
      //cout << endl << "Check AI RR_R" << endl;
      if(board[row][column]==player){
        if(connect2Plus1(board, player,player, ydim, xdim, row, column, y, x)){
          //cout << endl << "Check AI RR_R" << endl;
          return false;
        }
      }
    }
  }
  
  // If the above doesn't occur, AI will check again if it has a connect 3
  // Connect 3 means it can win, so it will try to play with this
  // If there's no connect 3, there's no imediate win and it should then block
  for (int row=0; row<ydim; row++){
    for (int column=0; column<xdim; column++){
      // If blank, do nothing
      //cout << endl << "Check AI connect 3" << endl;
      if(board[row][column]==player){
        if(checkConnectX(board, player, player, ydim, xdim, row, column, y, x, 3)){
          //                    ^^^^^^ the player to check for winning
          //cout << endl << "Check AI connect 3" << endl;
          return false;
        }
      }
    }
  }

  // Check if the oponent can win
  // Check for connect 3
  // We will create a separate function for this check
  // Go through each coordinate and check if there's a connect 3
  // If so, see if there's a way to block the start or end
  for (int row=0; row<ydim; row++){
    for (int column=0; column<xdim; column++){
      // If blank, do nothing
      //cout << endl << "Check opponent 3" << endl;
      if(board[row][column]==opponent){
       // cout << endl << "Check opponent 3 " << row << " " << column<< endl;
        if(checkConnectX(board, opponent,player, ydim, xdim, row, column, y, x, 3)){
          //cout << endl << "Check opponent 3" << endl;
          return false;
        }
      }
    }
  }

  // There's another way for the opponent to win (e.g. R _ R R) so we need to check that
  for (int row=0; row<ydim; row++){
    for (int column=0; column<xdim; column++){
      // If blank, do nothing
      //cout << endl << "Check opponent RR_R" <<endl;
      if(board[row][column]==opponent){
        if(connect2Plus1(board, opponent,player, ydim, xdim, row, column, y, x)){
          //cout << endl << "Check opponent RR_R" <<endl;
          return false;
        }
      }
    }
  }

  // If none of the above occurs, AI will pick a location based on whether or not it has the most connect X
  // We've already checked for connect 3, so we'll start checking for connect 2, then connect 1
  // Using the same method for blocking the other player, we check if AI can win at start or end
  for (int size=2; size>0; size--){
    for (int row=0; row<ydim; row++){
      for (int column=0; column<xdim; column++){
        // If blank, do nothing
        //cout << endl << "Check AI connect " << size << endl;
        if(board[row][column]==player){
          if(checkConnectX(board, player, player, ydim, xdim, row, column, y, x, size)){
            //cout << endl << "Check AI connect " << size << endl;
            return false;
          }
        }
      }
    }
  }

  // If code reaches here, it means AI doesn't have a winning position or 
  //  can't block the other player
  // Start from left to right and bottom to top and check for lowest blank spot
  // The strategy above ensure the pieces don't pile up, making them more spread
  //  out and create more connect X
  for (int row=0; row<ydim; row++){
    for (int column=0; column<xdim; column++){
      // If blank, put a piece there
      //cout << endl <<"Print Random" << endl;
      if(board[row][column]==BLANK){
        board[row][column] = player;
        *x = column;
        *y = row;
        //cout << endl <<"Print Random" << endl;
        return false;
      }
    }
  }
  
  
  // The program shouldn't reach this point
  return true;
}

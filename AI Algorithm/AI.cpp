//---------Additional Function for getUserAIInput()----------
bool checkConnectX(
  BoardValue** board, 
  BoardValue testPlayer/*player that's being checked for connect X*/,
  BoardValue currentPlayer,
  int ydim, int xdim, 
  int sy, int sx,
  int* y, int* x, 
  int size)
{
  // ***(FUNCTION DESCRIPTION)*** 
  // Check if there's any X connected pieces ("connect X" for short)
  // The function will mainly be used to check for 2 or 3 connected pieces
  // The variable "size" will tell the function how many pieces to check for
  // We can reuse some of hasWon() except check until X
  // The function will calculate a starting and ending
  //  coordinate to where the connect X start and end (the points
  //  don't include the connect X pieces) this is so the AI know
  //  where to start checking.
  // The function will then check the start and end to see if it can
  //  put a piece there and update the x and y coordinates
  const int NDIRS = 4;
  const int xDirDelta[NDIRS] = { 0, +1, -1, +1};
  const int yDirDelta[NDIRS] = {+1,  0, +1, +1};
  int currentPCount;
  
  // We go through every axis until there's a connect X
  //  and a piece can be placed at the start or end
  for (int axis=0; axis<4; axis++){
    // Check vert, hor, diags 
    // Reset count every time axis change
    currentPCount = 0; 
    int xStart=0, xEnd=0, yStart=0, yEnd=0;

    for (int dir=-1; dir<2; dir+=2){
      // Check 2 directions (e.g. forward then backward)
      // First time will subtract xDirDelta and yDirDelta
      // Second time will add them
      if (currentPCount == size-1){break;} // stop if X are connected

      for(int check=1; check<size; check++){
        // Check up to 2 pieces because the original one for sure matches
        int row = sy+(dir*yDirDelta[axis]*check);
        int column = sx+(dir*xDirDelta[axis]*check);
        // Check if row and column are within range
        // Otherwise it'll give segmentation fault
        if (column<0 || column>=xdim || row<0 || row>=ydim ){break;}
        if (board[row][column] != testPlayer){
          if(dir==-1){
            // If reaches here, there has not been a connect 3
            // But there isn't the player's piece here either
            // Make the last point our start and add end later
            xStart = column;
            yStart = row;
          }
          break;
        } 
        else {currentPCount++;}
        
        if (currentPCount == size-1){
          
          if(dir==-1){
            // If reach here that means there's a connect 3 in the
            //  first direction and we use the current point as start
            //  and the original point as end
            xStart = sx+(dir*xDirDelta[axis]*(check+1));
            yStart = sy+(dir*yDirDelta[axis]*(check+1));
            xEnd = sx+(xDirDelta[axis]);
            yEnd = sy+(yDirDelta[axis]);
          }
          else{
            // If reaches here that means the 2nd direction found a 
            //  connect 3, which means the 1st direction output a start
            //  already
            xEnd = sx+(dir*xDirDelta[axis]*(check+1));
            yEnd = sy+(dir*yDirDelta[axis]*(check+1));
          }
          break;
        }
      }
    }
    // Check if X pieces are connected in the axis before resetting count
    // If they are and the AI is able to place the piece then it will do that and quit the function
    if (currentPCount == size-1){
      // If there's connect 3 in this axis, we look at if there's a way 
      //  to put a piece at the start or end
      // If there's not a way then check the next axis
      // Check if column is within range
      // We don't need to check if row is in range because findYValue() ensure that
      if (xStart<xdim && xStart>=0 && findYValue(board,ydim,xStart)==yStart && yStart>=0 && yStart<ydim){
          // The last parameter checks for if there's an BLANK below the intended coordinate
          // We'll default to put the piece at the start first if possible
          // We'll only put it there if the there is a way to block it
          // i.e. if there's a blank at start and everything bellow it is filled
          board[yStart][xStart] = currentPlayer;
          *y = yStart;
          *x = xStart;
          return true;
        
      }
      else if (xEnd<xdim && xEnd>=0 && findYValue(board, ydim, xEnd) == yEnd && yEnd>=0 && yEnd<ydim){
        // If we can't put the piece at the start then we will try the end
        board[yEnd][xEnd] = currentPlayer;
        *y = yEnd;
        *x = xEnd;
        return true;
      }
    }
  }
  // If code reaches here, there's no connect X
  return false;
}

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
      if ((sx+check*col)<0 || (sx+check*col)>=xdim || (sy+check*row)<0 || (sy+check*row)>=ydim||((sy+check*row)-1)<0 ||((sy+check*row)-1)>=ydim)
      {return false;}
    }

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

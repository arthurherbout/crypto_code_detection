#include <fstream>
#include <iostream>
#include <vector>

class Grid {
  public:
    bool FindLowestNeighborLabel(size_t row, size_t col, char&  label) {
      bool ret = false;
      size_t lowestAltitude = m_grid[row][col].altitude;
      size_t lowestRow;
      size_t lowestCol;
      if(row > 0) {
        if(m_grid[row-1][col].altitude < lowestAltitude) {
          lowestAltitude = m_grid[row-1][col].altitude;
          lowestRow = row-1;
          lowestCol = col;
          ret = true;
        }
      }

      if(col > 0) {
        if(m_grid[row][col-1].altitude < lowestAltitude) {
          lowestAltitude = m_grid[row][col-1].altitude;
          lowestRow = row;
          lowestCol = col-1;
          ret = true;
        }
      }

      if(col + 1 < m_width) {
        if(m_grid[row][col+1].altitude < lowestAltitude) {
          lowestAltitude = m_grid[row][col+1].altitude;
          lowestRow = row;
          lowestCol = col+1;
          ret = true;
        }
      }

      if(row + 1 < m_height) {
        if(m_grid[row+1][col].altitude < lowestAltitude) {
          lowestAltitude = m_grid[row+1][col].altitude;
          lowestRow = row+1;
          lowestCol = col;
          ret = true;
        }
      }

      if(ret) {
        LabelCell(lowestRow, lowestCol);
        label = m_grid[lowestRow][lowestCol].label;
      }
      return ret;
    }

    void LabelCell(size_t row, size_t col) {
      if(m_grid[row][col].isLabeled)
        return;

      char label;
      if(FindLowestNeighborLabel(row, col, label)) {
        m_grid[row][col].label = label;
      } else {
        m_grid[row][col].label = m_nextAvailableLabel++;
      }
      m_grid[row][col].isLabeled = true;
    }

    void LabelCells() {
      for(size_t curRow = 0; curRow < m_height; curRow++) {
        for(size_t curCol = 0; curCol < m_width; curCol++) {
          LabelCell(curRow, curCol);
        }
      }
    }

    void Print() {
      for(size_t curRow = 0; curRow < m_height; curRow++) {
        for(size_t curCol = 0; curCol < m_width; curCol++) {
          std::cout << m_grid[curRow][curCol].label << " ";
        }
        std::cout << std::endl;
      }
    }

    Grid(std::ifstream& stream) : m_nextAvailableLabel('a') {
      stream >> m_height >> m_width;
      m_grid.resize(m_height);
      for(size_t curRow = 0; curRow < m_height; curRow++) {
        m_grid[curRow].resize(m_width);
        for(size_t curCol = 0; curCol < m_width; curCol++) {
          stream >> m_grid[curRow][curCol].altitude;
          m_grid[curRow][curCol].isLabeled = false;
        }
      }
    }

  private:
    typedef struct {
      size_t altitude;
      bool isLabeled;
      char label;
    } GridCell;

    size_t m_height;
    size_t m_width;
    std::vector<std::vector<GridCell> > m_grid;
    char m_nextAvailableLabel;
};

int main(int argc, char* argv[]) {
    std::ifstream fileStream(argv[1]);

    size_t numMaps;
    fileStream >> numMaps;

    for(size_t currentMap = 1; currentMap <= numMaps; currentMap++) {
      Grid grid(fileStream);
      grid.LabelCells();
      std::cout << "Case #" << currentMap << ": " << std::endl;
      grid.Print();
    }
}

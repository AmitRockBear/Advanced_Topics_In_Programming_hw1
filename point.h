#ifndef POINT_H
#define POINT_H

class Point {
private:
    int x;
    int y;

public:
    Point(int x, int y);

    int getX() const;
    void setX(int newX);
    int getY() const;
    void setY(int newY);
};

#endif // POINT_H

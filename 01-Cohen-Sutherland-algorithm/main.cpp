#include <iostream>
using namespace std;

class CuhenSutherlandClipper {
private:
	struct Point {
		double x;
		double y;
		Point() = default;
		Point(double x, double y) :x(x), y(y) {
		}
		Point(Point& other) {
			x = other.x;
			y = other.y;
		}		
	};
	struct LineSegment {
		Point* start;	// �˵�
		Point* end;		// �˵�
		LineSegment() = default;
		// 
		LineSegment(double x0, double y0, double x1, double y1) {
			start = new Point(x0, y0);
			end = new Point(x1, y1);
		}
		LineSegment(LineSegment& other) {			
			start = new Point(*other.start);
			end = new Point(*other.end);
		}
		void setStart(double x, double y) {
			start->x = x;
			start->y = y;
		}
		void setEnd(double x, double y) {
			end->x = x;
			end->y = y;
		}
		double getStartX() const {
			return start->x;
		}
		double getStartY() const {
			return start->y;
		}
		double getEndX() const {
			return end->x;
		}
		double getEndY() const {
			return end->y;
		}
	};
	struct Rectangle {
		double xmin;
		double xmax;
		double ymin;
		double ymax;
		Rectangle() = default;
		// ���½Ƕ���  ���ο�ȡ��߶�
		Rectangle(double xmin, double ymin, double clipw, double cliph) :
			xmin(xmin), ymin(ymin), xmax(xmin + clipw), ymax(ymin + cliph) {}
	};
private:
	LineSegment* lineSegment;
	LineSegment* resultLine;
	Rectangle* rect;
	bool accept;
public:
	CuhenSutherlandClipper() = default;
	// ǰ��4������ֱ�߶����˵�����꣬����5��6�Ǿ��ε����½Ƕ������꣬7��8�Ǿ��εĿ�Ⱥͳ���
	CuhenSutherlandClipper(double x0, double y0, double x1, double y1,
		double clipx, double clipy, double clipw, double cliph) {
		lineSegment = new LineSegment(x0, y0, x1, y1);
		rect = new Rectangle(clipx, clipy, clipw, cliph);
		encodeEndpoint(*lineSegment->start);
		encodeEndpoint(*lineSegment->end);
		resultLine = nullptr;
		accept = false;
	}
	/*
	 * ���вü�
	   true������ֱ�߶�������н���������ھ�����
	   false����ʾ���߶β��ھ��η�Χ��
	 */
	bool lineClipped() {
		//bool accept = false;
		do {
			// get endpoints code
			int e0code = encodeEndpoint(*lineSegment->start); 
			int e1code = encodeEndpoint(*lineSegment->end);

			if ((e0code | e1code )== 0) {
				accept = true;
				resultLine = new LineSegment(*lineSegment);
				break;
			}
			else if ((e0code & e1code) != 0) {
				accept = false;
				break;
			}
			else {
				// ѡ��һ���ڲü����δ���֮��Ķ˵�
				int code = e0code != 0 ? e0code : e1code;

				double newx, newy;
				double x0 = lineSegment->getStartX();
				double y0 = lineSegment->getStartY();
				double x1 = lineSegment->getEndX();
				double y1 = lineSegment->getEndY();
				// �˵��ھ��δ��ڵ����
				if ((code & (1 << 0)) != 0) {
					newx = rect->xmin;
					/*
					**    y1- y0 / x1 - x0 = y - y0 / x - x0
					*
					*/
					newy = ((y1 - y0) / (x1 - x0)) * (newx - x0) + y0;
				}
				// �Ҳ�
				else if ((code & (1 << 1)) != 0) {
					newx = rect->xmax;
					newy = ((y1 - y0) / (x1 - x0)) * (newx - x0) + y0;					
				}
				// �²�
				else if ((code & (1 << 2)) != 0) {
					newy = rect->ymin;
					newx = ((x1 - x0) / (y1 - y0)) * (newy - y0) + x0;
				}
				// �ϲ�
				else if ((code & (1 << 3)) != 0) {
					newy = rect->ymax;
					newx = ((x1 - x0) / (y1 - y0)) * (newy - y0) + x0;
				}
				/* Now we replace the old endpoint depending on which we chose */
				if (code == e0code) {
					lineSegment->setStart(newx, newy);
				}
				else {
					lineSegment->setEnd(newx, newy);
				}
			}
		} while (true);

		return accept;
	}
	// ���ֱ�߶����ھ�����������ü�֮���ֱ�߶Σ���������ü�Ϊ��
	void print() {
		if (accept) {
			cout << "��ֱ�߶����ھ��δ����ڵĲ��ֵ�����Ϊ ��" << endl;
			cout << "x0 = " << resultLine->getStartX() << endl;
			cout << "y0 = " << resultLine->getStartY() << endl;
			cout << "x1 = " << resultLine->getEndX() << endl;
			cout << "y1 = " << resultLine->getEndY() << endl;
		}
		else {
			cout << "ֱ�߶β�δ���ھ��δ���֮��" << endl;
		}
	}
	// ����ֱ�߶ε�����
	void setLineSegment(double x0, double y0, double x1, double y1) {
		if (lineSegment != nullptr) {
			lineSegment->setStart(x0, y0);
			lineSegment->setEnd(x1, y1);
		}
		else {
			lineSegment = new LineSegment(x0, y0, x1, y1);
		}
	}
private:
	/*
	 * �Զ˵�ֵ���ݾ��ν��б���
	 */
	int encodeEndpoint( Point& po) {
		int code = 0;
		if (po.x < rect->xmin) {
			code |= (1 << 0);
		}
		else if (po.x > rect->xmax) {
			code |= (1 << 1);
		}

		if (po.y < rect->ymin) {
			code |= (1 << 2);
		}
		else if (po.y > rect->ymax) {
			code |= (1 << 3);
		}
		return code;
	}

};

int main() {
	CuhenSutherlandClipper clipper(0.5, 1.5, 1, 0, 0, 0, 1, 1);
	if (clipper.lineClipped()) {
		clipper.print();
	}
	else {
		cout << "ֱ�߶β�δ���ھ��δ���֮��" << endl;
	}
	clipper.setLineSegment(0.5, 1.5, 2, 3);
	if (clipper.lineClipped()) {
		clipper.print();
	}
	else {
		cout << "ֱ�߶β�δ���ھ��δ���֮��" << endl;
	}
	//cout << "hello world" << endl;
	system("pause");
	return 0;
}

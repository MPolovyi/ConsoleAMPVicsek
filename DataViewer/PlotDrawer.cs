using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;

namespace DataViewer
{
    internal class PlotDrawer
    {
        public void DrawLines(Point leftTop, Point rightBottom,
            double minValueX, double maxValueX, double stepX,
            double minValueY, double maxValueY, double stepY,
            Path path)
        {
            double graphStepX = ((rightBottom.X - leftTop.X) / (maxValueX - minValueX)) * stepX;
            double graphStepY = ((rightBottom.Y - leftTop.Y) / (maxValueY - minValueY)) * stepY;
            DrawHorisontalLines(leftTop, rightBottom, graphStepY, path);
            DrawVerticalLines(leftTop, rightBottom, graphStepX, path);
        }

        private void DrawHorisontalLines(Point leftTop, Point rightBottom, double step, Path path)
        {
            double y = rightBottom.Y;
            var geometryGroup = new GeometryGroup();

            while (y >= leftTop.Y)
            {
                var geom = new LineGeometry(new Point(leftTop.X, y), new Point(rightBottom.X, y));
                geometryGroup.Children.Add(geom);
                y -= step;
            }
            if (path.Data != null)
                geometryGroup.Children.Add(path.Data);

            path.Data = geometryGroup;
        }

        private void DrawVerticalLines(Point leftTop, Point rightBottom, double step, Path path)
        {
            double x = leftTop.X;
            var geometryGroup = new GeometryGroup();

            while (x <= rightBottom.X)
            {
                var geom = new LineGeometry(new Point(x, leftTop.Y), new Point(x, rightBottom.Y));
                geometryGroup.Children.Add(geom);
                x += step;
            }
            if (path.Data != null)
                geometryGroup.Children.Add(path.Data);

            path.Data = geometryGroup;
        }
    }
}
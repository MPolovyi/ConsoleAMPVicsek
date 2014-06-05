using System.Globalization;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;

namespace DataViewer
{
    internal class GridDrawer
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
            DrawTextLabels(leftTop, rightBottom, minValueX, stepX, graphStepX, minValueY, stepY, graphStepY, path);
        }

        private void DrawTextLabels(Point leftTop, Point rightBottom,
            double minValueX, double stepX, double graphStepX,
            double minValueY, double stepY, double graphStepY,
            Path path)
        {
            double x = leftTop.X + 10;
            double y = leftTop.Y + 15;
            var geometryGroup = new GeometryGroup();
            
            while (y <= rightBottom.Y)
            {
                var text = new FormattedText(minValueY.ToString(minValueY > 2 ? "###" : "##.###"),
                    CultureInfo.CurrentCulture, FlowDirection.LeftToRight,
                    new Typeface("Arial"), 10,
                    Brushes.Black);
                geometryGroup.Children.Add(text.BuildGeometry(new Point(10, rightBottom.Y - y)));
                y += graphStepY;
                minValueY += stepY;
            }

            y = rightBottom.Y - 15;
            while (x <= rightBottom.X)
            {
                var text = new FormattedText(minValueX.ToString(minValueX > 2 ? "###" : "##.###"),
                    CultureInfo.CurrentCulture, FlowDirection.LeftToRight,
                    new Typeface("Arial"), 10,
                    Brushes.Black);
                geometryGroup.Children.Add(text.BuildGeometry(new Point(x, y)));
                x += graphStepX;
                minValueX += stepX;
            }
            if (path.Data != null)
                geometryGroup.Children.Add(path.Data);

            path.Data = geometryGroup;
        }

        private void DrawHorisontalLines(Point leftTop, Point rightBottom, double step, Path path)
        {
            double y = leftTop.Y;
            var geometryGroup = new GeometryGroup();

            while (y <= rightBottom.Y)
            {
                var geom = new LineGeometry(new Point(leftTop.X, rightBottom.Y - y), new Point(rightBottom.X, rightBottom.Y - y));
                geometryGroup.Children.Add(geom);
                y += step;
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
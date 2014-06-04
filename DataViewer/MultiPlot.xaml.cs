using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.TextFormatting;
using System.Windows.Shapes;

namespace DataViewer
{
    /// <summary>
    /// Interaction logic for MultyPlot.xaml
    /// </summary>
    public partial class MultiPlot : UserControl
    {
        private List<List<Tuple<double, double>>> _loadedSeries = new List<List<Tuple<double, double>>>();
        private GridDrawer _drawer = new GridDrawer();
        private List<string> _seriesComments = new List<string>();
        private double _stepX = 1;
        private double _stepY = 1;
        private bool _firstLoaded = false;

        public MultiPlot()
        {
            InitializeComponent();
        }

        public MultiPlot(double height, double width)
            : this()
        {
            MyCanvas.Height = height;
            MyCanvas.Width = width;
        }

        public void AddPlotSeries(List<Tuple<double, double>> ansv, string comment, double stepX = 1, double stepY = 1)
        {
            _seriesComments.Add(comment);
            AddPlotSeries(ansv, _firstLoaded ? _stepX : stepX, _firstLoaded ? _stepY : stepY);
            ActiveLabel.Content = comment;
            _firstLoaded = true;
            _loadedSeries.Add(ansv);
        }

        private void AddPlotSeries(List<Tuple<double, double>> ansv, double stepX, double stepY)
        {
            _stepX = stepX;
            _stepY = stepY;
            if (!_firstLoaded)
                DrawGrid(ansv.Min(item => item.Item1), ansv.Max(item => item.Item1), stepX,
                    ansv.Min(item => item.Item2), ansv.Max(item => item.Item2), stepY);
            MyCanvas.Children.Add(GenerateSeriesPath(ansv));
        }

        private Path GenerateSeriesPath(List<Tuple<double, double>> ansv)
        {
            var myPath = new Path
            {
                Height = MyCanvas.Height,
                Width = MyCanvas.Width,
                Stroke = Brushes.Black,
                StrokeThickness = 3
            };
            myPath.MouseLeftButtonUp += (sender, args) =>
            {
                if (Equals((sender as Path).Stroke, Brushes.Black))
                {
                    (sender as Path).Stroke = Brushes.Red;
                    ActiveLabel.Content = _seriesComments[MyCanvas.Children.IndexOf(sender as Path) - 3];
                }
                else
                {
                    (sender as Path).Stroke = Brushes.Black;
                    ActiveLabel.Content = "Select plot for further info";
                }
            };
            var segment = new PolyBezierSegment();
            double scaleY = MyCanvas.Height / ansv.Max(item => item.Item2);
            double scaleX = MyCanvas.Width / ansv.Max(item => item.Item1);
            for (int i = ansv.Count - 1; i >= 0; i--)
            {
                segment.Points.Add(new Point(ansv[i].Item1 * scaleX, MyCanvas.Height - ansv[i].Item2 * scaleY));
            }
            var fig = new PathFigure();
            fig.Segments.Add(segment);
            fig.StartPoint = segment.Points.First();
            var geom = new PathGeometry();
            geom.Figures.Add(fig);
            myPath.Data = geom;
            return myPath;
        }

        private void DrawGrid(double minX, double maxX, double stepX, double minY, double maxY, double stepY)
        {
            var tmpPath = new Path
            {
                Height = MyCanvas.Height,
                Width = MyCanvas.Width / 2 - 10,
                Stroke = Brushes.Black,
                StrokeThickness = 1
            };
            _drawer.DrawLines(new Point(0, 0), new Point(MyCanvas.Width, MyCanvas.Height),
                minX, maxX, stepX,
                minY, maxY, stepY,
                tmpPath);
            MyCanvas.Children.Insert(0, tmpPath);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (_loadedSeries.Count >= 2)
            {
                var data = from series in _loadedSeries
                    from t1 in series
                    group t1 by t1.Item1
                    into grp
                    select grp;

                var tmp = data.Select(it => new Tuple<double, double>(it.Key, it.Average(z => z.Item2))).ToList();

                _loadedSeries.Insert(0, tmp);
            }
            Redraw();
        }

        private void Redraw()
        {
            MyCanvas.Children.RemoveRange(3, MyCanvas.Children.Count - 3);
            _seriesComments.RemoveAll(it => true);
            AddPlotSeries(_loadedSeries[0], "Averaged veloc on all data loaded previously");
        }
    }
}
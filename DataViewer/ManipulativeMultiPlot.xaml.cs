using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Shapes;

namespace DataViewer
{
    /// <summary>
    /// Interaction logic for ManipulativeMultiPlot.xaml
    /// </summary>
    public partial class ManipulativeMultiPlot : UserControl
    {
        private double _index = 0;
        private PlotDrawer _drawer = new PlotDrawer();
        private List<string> _seriesComments = new List<string>();
        private List<List<Tuple<List<Tuple<double, double>>, double>>> _XandYandParam = new List<List<Tuple<List<Tuple<double, double>>, double>>>();
        private double _stepX = 1;
        private double _stepY = 1;
        private bool _firstLoaded = false;

        public ManipulativeMultiPlot()
        {
            InitializeComponent();
        }

        public ManipulativeMultiPlot(double height, double width)
            : this()
        {
            MyCanvas.Height = height;
            MyCanvas.Width = width;
            MySlider.Width = width - 20;
        }

        public void AddPlotSeries(List<Tuple<List<Tuple<double, double>>, double>> ansv, string comment, double stepX = 1, double stepY = 1)
        {
            _seriesComments.Add(comment);
            AddPlotSeries(ansv, _firstLoaded ? _stepX : stepX, _firstLoaded ? _stepY : stepY);
            MyActiveLabel.Content = comment;
            _firstLoaded = true;
        }

        private void AddPlotSeries(List<Tuple<List<Tuple<double, double>>, double>> ansv, double stepX, double stepY)
        {
            _stepX = stepX;
            _stepY = stepY;
            if (!_firstLoaded)
            {
                DrawGrid(ansv.Min(item => item.Item1.Min(it => it.Item1)), ansv.Max(item => item.Item1.Max(it => it.Item1)), stepX,
                    ansv.Min(item => item.Item1.Min(it => it.Item2)), ansv.Max(item => item.Item1.Max(it => it.Item2)), stepY);
                PopulateSlider(ansv.Min(it => it.Item2), ansv.Max(it => it.Item2),
                    (from a in ansv from b in ansv let d = Math.Abs(a.Item2 - b.Item2) where a.Item1 != b.Item1 select d).Min());
            }
            _XandYandParam.Add(ansv);
            Redraw();
        }

        private void PopulateSlider(double min, double max, double d)
        {
            MySlider.Minimum = min;
            MySlider.Maximum = max;
            MySlider.SmallChange = d;
            MySlider.LargeChange = d;
            MySlider.TickFrequency = d;
            MySlider.IsSnapToTickEnabled = true;
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
                    MyActiveLabel.Content = _seriesComments[MyCanvas.Children.IndexOf(sender as Path) - 4];
                }
                else
                {
                    (sender as Path).Stroke = Brushes.Black;
                    MyActiveLabel.Content = "Select plot for further info";
                }
            };

            if (ansv == null) return myPath;
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

        private void MySlider_OnValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            _index = e.NewValue;
            Redraw();
        }

        private void Redraw()
        {
            MyParamLabel.Content = _index;
            List<List<Tuple<double, double>>> tmpSeries = new List<List<Tuple<double, double>>>();

            foreach (var serie in _XandYandParam)
            {
                var tmp = from tuple in serie
                          where Math.Abs(tuple.Item2 - _index) < double.Epsilon
                          select tuple.Item1;
                tmpSeries.AddRange(tmp);
            }

            //Preserve slider and lable!
            MyCanvas.Children.RemoveRange(4, MyCanvas.Children.Count - 4);
            foreach (var tmp in tmpSeries)
            {
                MyCanvas.Children.Add(GenerateSeriesPath(tmp));
            }
        }
    }
}
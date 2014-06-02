using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;
using Microsoft.Win32;

namespace DataViewer
{
    /// <summary>
    ///     Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private bool _spdByHLoaded;

        private List<Tuple<List<double>, double>> _speedByHeight = new List<Tuple<List<double>, double>>();
        private List<Tuple<double, double>> _speedByTime = new List<Tuple<double, double>>();
        private DataGetter _getter = new DataGetter();
        private PlotDrawer _drawer = new PlotDrawer();
        private MultiPlot _plot1;
        private ManipulativeMultiPlot _plot2;

        public MainWindow()
        {
            InitializeComponent();
            _plot1 = new MultiPlot(ImgPanel.Height, ImgPanel.Width / 2 - 10);
            _plot2 = new ManipulativeMultiPlot(ImgPanel.Height, ImgPanel.Width / 2 - 10);
            _plot1.Margin = new Thickness(0, 0, 10, 0);
            _plot2.Margin = new Thickness(0, 10, 0, 0);
            ImgPanel.Children.Add(_plot1);
            ImgPanel.Children.Add(_plot2);
        }

        private void DrawSpdByHeight(List<Tuple<List<double>, double>> ansv, int index)
        {
            var tmp = new List<Tuple<List<Tuple<double, double>>, double>>();
            for (int i = 0; i < ansv[0].Item1.Count ; i++)
            {
                var tempList = new List<Tuple<double, double>>();
                for (int j = 0; j < ansv.Count; j++)
                {
                    tempList.Add(new Tuple<double, double>(ansv[j].Item1[i], j));
                }
                tmp.Add(new Tuple<List<Tuple<double, double>>, double>(tempList, i));
            }
            _plot2.AddPlotSeries(tmp, "Test plot", 0.1, 20);
        }

        private void DrawSpdByNoise(List<Tuple<double, double>> ansv)
        {
            _plot1.AddPlotSeries(ansv, "TestPlot", (ansv.Max(item => item.Item1) - ansv.Min(item=>item.Item1))/10, 0.1);
        }

        private Canvas CreatePlot(List<Tuple<double, double>> ansv)
        {
            var canvas = new Canvas()
            {
                Height = ImgPanel.Height,
                Width = ImgPanel.Width/2 - 10,
                Margin = new Thickness(0, 0, 10, 0)
            };
            var myPath = new System.Windows.Shapes.Path
            {
                Height = ImgPanel.Height,
                Width = ImgPanel.Width/2 - 10,
                Margin = new Thickness(0, 0, 10, 0),
                Stroke = Brushes.Red,
                StrokeThickness = 1
            };
            
            var segment = new PolyBezierSegment();
            double scaleY = ImgPanel.Height/ansv.Max(item => item.Item1);
            double scaleX = myPath.Width/ansv.Count;
            for (int i = ansv.Count - 1; i >= 0; i--)
            {
                segment.Points.Add(new Point(ansv[i].Item2*scaleX, ImgPanel.Height - ansv[i].Item1*scaleY));
            }
            var fig = new PathFigure();
            fig.Segments.Add(segment);
            fig.StartPoint = segment.Points.First();
            var geom = new PathGeometry();
            geom.Figures.Add(fig);
            myPath.Data = geom;
            var tmpPath = new System.Windows.Shapes.Path
            {
                Height = ImgPanel.Height,
                Width = ImgPanel.Width/2 - 10,
                Margin = new Thickness(0, 0, 10, 0),
                Stroke = Brushes.Black,
                StrokeThickness = 1
            };
            _drawer.DrawLines(new Point(0, 0), new Point(myPath.Width, ImgPanel.Height),
                0, 360, 30,
                0, 1, 0.1,
                tmpPath);
            canvas.Children.Add(myPath);
            canvas.Children.Add(tmpPath);
            return canvas;
        }

        private void LoadSpdByHeightFile(OpenFileDialog openDlg)
        {
            if (true == openDlg.ShowDialog())
            {
                var dataFromFile = new StreamReader(openDlg.FileName);
                _speedByHeight = _getter.GetSpeedByHeight(dataFromFile);
                _spdByHLoaded = true;
                DrawSpdByHeight(_speedByHeight, 0);
                SimDataLabel.Content = _getter.GetSimParams(dataFromFile);
            }
        }

        private void LoadSpdByNoiseFile(OpenFileDialog openDlg)
        {
            if (true == openDlg.ShowDialog())
            {
                var dataFromFile = new StreamReader(openDlg.FileName);
                _speedByTime = _getter.GetSpeedByNoise(dataFromFile);
                DrawSpdByNoise(_speedByTime);
                SimDataLabel.Content = _getter.GetSimParams(dataFromFile);
            }
        }

        private void OpenAllButton_Click(object sender, RoutedEventArgs e)
        {
            // Create an open file dialog box and only show XAML files.
            var openDlg = new OpenFileDialog { Filter = "Text Files |*.txt" };

            LoadSpdByNoiseFile(openDlg);
            LoadSpdByHeightFile(openDlg);
        }

        private void OpenSpdByHeightButton_Click(object sender, RoutedEventArgs e)
        {
            var openDlg = new OpenFileDialog { Filter = "Text Files |*.txt" };
            LoadSpdByHeightFile(openDlg);
        }

        private void OpenSpdByNoiseButton_Click(object sender, RoutedEventArgs e)
        {
            var openDlg = new OpenFileDialog { Filter = "Text Files |*.txt" };
            LoadSpdByNoiseFile(openDlg);
        }

        private void RangeBase_OnValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int index = Convert.ToInt32(e.NewValue);
            if (_spdByHLoaded)
            {
                DrawSpdByHeight(_speedByHeight, index);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (_spdByHLoaded)
            {
                for (int i = 0; i < _speedByHeight[0].Item1.Count; i++)
                {
                    double max = _speedByHeight.Max(item => item.Item1[i]);
                    foreach (var tuple in _speedByHeight)
                    {
                        tuple.Item1[i] /= max;
                    }
                }
            }
        }

        private void AddSpdByNoiseButton_Click(object sender, RoutedEventArgs e)
        {
            
        }
    }
}
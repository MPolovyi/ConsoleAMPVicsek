using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Media;
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

        public MainWindow()
        {
            InitializeComponent();
        }

        private void DrawSpdByHeight(List<Tuple<List<double>, double>> ansv, int index)
        {
            if (index > ansv[0].Item1.Count())
            {
                return;
            }
            var segment = new PolyBezierSegment();
            double scaleY = ImgPanel.Height / ansv.Count;
            double scaleX = MyPath2.Width / ansv.Max(item => item.Item1.Max());
            for (int i = 0; i < ansv.Count; i++)
            {
                segment.Points.Add(new Point(ansv[i].Item1[index] * scaleX, i * scaleY));
            }
            var fig = new PathFigure();
            fig.Segments.Add(segment);
            fig.StartPoint = segment.Points.First();
            var geom = new PathGeometry();
            geom.Figures.Add(fig);
            MyPath2.Stroke = Brushes.Black;
            MyPath2.StrokeThickness = 5;
            MyPath2.Data = geom;
            var str = string.Format("Noise = {0}, Max Veloc = {1}", index, ansv.Max(item => item.Item1.Max()));
            NoiseLabel.Content = str;
        }

        private void DrawSpdByNoise(List<Tuple<double, double>> ansv)
        {
            var segment = new PolyBezierSegment();
            double scaleY = ImgPanel.Height / ansv.Max(item => item.Item1);
            double scaleX = MyPath.Width / ansv.Count;
            for (int i = ansv.Count - 1; i >= 0; i--)
            {
                segment.Points.Add(new Point(ansv[i].Item2 * scaleX, ImgPanel.Height - ansv[i].Item1 * scaleY));
            }
            var fig = new PathFigure();
            fig.Segments.Add(segment);
            fig.StartPoint = segment.Points.First();
            var geom = new PathGeometry();
            geom.Figures.Add(fig);
            MyPath.Stroke = Brushes.Black;
            MyPath.StrokeThickness = 5;
            MyPath.Data = geom;
        }

        private void LoadSpdByHeightFile(OpenFileDialog openDlg)
        {
            if (true == openDlg.ShowDialog())
            {
                var dataFromFile = new StreamReader(openDlg.FileName);
                var getter = new DataGetter();

                _speedByHeight = getter.GetSpeedByHeight(dataFromFile);
                _spdByHLoaded = true;
                DrawSpdByHeight(_speedByHeight, 0);
                SimDataLabel.Content = getter.GetSimParams(dataFromFile);
            }
        }

        private void LoadSpdByNoiseFile(OpenFileDialog openDlg)
        {
            if (true == openDlg.ShowDialog())
            {
                var dataFromFile = new StreamReader(openDlg.FileName);
                var getter = new DataGetter();
                _speedByTime = getter.GetSpeedByNoise(dataFromFile);
                DrawSpdByNoise(_speedByTime);
                SimDataLabel.Content = getter.GetSimParams(dataFromFile);
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
    }
}
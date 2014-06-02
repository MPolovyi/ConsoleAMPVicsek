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

        private void DrawSpdByHeight(List<Tuple<List<double>, double>> ansv, string comment)
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
            _plot2.AddPlotSeries(tmp, comment, 0.1, 20);
        }

        private void DrawSpdByNoise(List<Tuple<double, double>> ansv, string comment)
        {
            _plot1.AddPlotSeries(ansv, comment, (ansv.Max(item => item.Item1) - ansv.Min(item=>item.Item1))/10, 0.1);
        }
        
        private void LoadSpdByHeightFile(OpenFileDialog openDlg)
        {
            if (true == openDlg.ShowDialog())
            {
                var dataFromFile = new StreamReader(openDlg.FileName);
                _speedByHeight = _getter.GetSpeedByHeight(dataFromFile);
                _spdByHLoaded = true;
                DrawSpdByHeight(_speedByHeight, _getter.GetSimParams(dataFromFile));
            }
        }

        private void LoadSpdByNoiseFile(OpenFileDialog openDlg)
        {
            if (true == openDlg.ShowDialog())
            {
                var dataFromFile = new StreamReader(openDlg.FileName);
                _speedByTime = _getter.GetSpeedByNoise(dataFromFile);
                DrawSpdByNoise(_speedByTime, _getter.GetSimParams(dataFromFile));
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
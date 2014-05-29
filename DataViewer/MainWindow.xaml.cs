using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using Path = System.IO.Path;

namespace DataViewer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void OpenButton_Click(object sender, RoutedEventArgs e)
        {
            // Create an open file dialog box and only show XAML files. 
            OpenFileDialog openDlg = new OpenFileDialog();
            openDlg.Filter = "Text Files |*.txt";

            // Did they click on the OK button?
            if (true == openDlg.ShowDialog())
            {
                // Load all text of selected file into a StringBuilder.
                StreamReader dataFromFile = new StreamReader(openDlg.FileName);
                DataConverter converter = new DataConverter();
                var ansv = converter.GetSpeedOfTime(dataFromFile);
                
                PolyBezierSegment segment = new PolyBezierSegment();
                
                for (int i = ansv.Count-1; i >= 0; i--)
                {
                    segment.Points.Add(new Point(ansv[i].Item2,ansv[i].Item1 * 500));
                }
                PathFigure fig = new PathFigure();
                
                fig.Segments.Add(segment);
                PathGeometry geom = new PathGeometry();
                geom.Figures.Add(fig);
                MyPath.Stroke = Brushes.Black;
                MyPath.StrokeThickness = 1;
                var mySolidColorBrush = new SolidColorBrush();
                mySolidColorBrush.Color = Colors.Black;
                MyPath.Fill = mySolidColorBrush;
                MyPath.Data = geom;
            }
        }
    }
}

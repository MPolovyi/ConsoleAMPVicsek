using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Vicsek.Classes;

namespace Vicsek.Properties
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Strategy.Init(1000, pictureBox1);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            Strategy.AddParticles();
        }
    }
}

using System;
using System.Windows.Forms;
using Vicsek.Classes;

namespace Vicsek.Forms
{
    public partial class MainForm : Form
    {
        private double MaxSpeedAchieved = 0;
        private double m_particles = 0;


        public MainForm()
        {
            InitializeComponent();

            textBoxParticleCount.KeyUp += onTextBoxParticleCountOnKeyUp;
            textBoxParticleCount.Text = Miscelaneous.ParticleCount.ToString();
        }

        private void onTextBoxParticleCountOnKeyUp(object _sender, KeyEventArgs _args)
        {
            if (_args.KeyData == Keys.Enter)
            {
                try
                {
                    var newCount = Convert.ToInt32(((TextBox) _sender).Text);
                    Miscelaneous.ParticleCount = newCount;
                }
                catch
                {
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Init();
            Strategy.Init(pictureBox1);            
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            Strategy.AddParticles(100);
        }

        private void Init()
        {
            InitEvents();
            InitData();
        }

        private void InitData()
        {
            InitSpeed(textBoxSpeed);
            InitParticleCount(textBoxParticleCount);
            UpdNoize(textBoxNoize);
            InitRadius(textBoxInterractionR);
        }

        private void InitRadius(TextBox _textBox)
        {
            if (String.Compare(_textBox.Text, " ", StringComparison.Ordinal) == 0)
            {
                _textBox.Text = Miscelaneous.InterractionR.ToString();
            }
            else
            {
                var intR = Convert.ToDouble(_textBox.Text);
                Miscelaneous.InterractionR = intR;
            }
            trackBarInterractionR.Minimum = 1;
            trackBarInterractionR.Value = trackBarInterractionR.Maximum = (int) (1000*Miscelaneous.InterractionR);
        }

        private void InitSpeed(TextBox _textBox)
        {
            if (String.Compare(_textBox.Text, " ", StringComparison.Ordinal) == 0)
            {
                MaxSpeedAchieved = Miscelaneous.ParticleSpeed;
                _textBox.Text = MaxSpeedAchieved.ToString();
            }
            else
            {
                var spd = Convert.ToDouble(_textBox.Text);
                MaxSpeedAchieved = Miscelaneous.ParticleSpeed = spd;
            }
            trackBarSpeed.Minimum = 1;
            trackBarSpeed.Value = trackBarSpeed.Maximum = (int)(1000*Miscelaneous.ParticleSpeed);
        }

        private void InitParticleCount(TextBox _textBox)
        {
            if (String.Compare(_textBox.Text, " ", StringComparison.Ordinal) == 0)
            {
                _textBox.Text = Miscelaneous.ParticleCount.ToString();
            }
            else
            {
                Miscelaneous.ParticleCount = Convert.ToInt32(_textBox.Text);
            }

            m_particles = Miscelaneous.ParticleCount;
        }

        private void UpdNoize(TextBox _textBox)
        {
            if (String.Compare(_textBox.Text, " ", StringComparison.Ordinal) == 0)
            {
                _textBox.Text = Miscelaneous.Noize.ToString();
            }
            else
            {
                var noize = Convert.ToInt32(_textBox.Text);

                Miscelaneous.Noize = noize < 180 ? noize : 179;
            }
            trackBarNoize.Maximum = 180;
            trackBarNoize.Value = Miscelaneous.Noize;
        }

        
        private void InitEvents()
        {
            textBoxNoize.KeyUp += (_sender, _args) =>
                {
                    if (_args.KeyData == Keys.Enter)
                    {
                        UpdNoize((TextBox) _sender);
                    }
                };
            trackBarNoize.ValueChanged += (_sender, _args) =>
                {
                    Miscelaneous.Noize = ((TrackBar) _sender).Value;
                    textBoxNoize.Text = Miscelaneous.Noize.ToString();
                };

            textBoxParticleCount.KeyUp -= onTextBoxParticleCountOnKeyUp;
            textBoxParticleCount.KeyUp += (_sender, _args) =>
                {

                    if (_args.KeyData == Keys.Enter)
                    {
                        try
                        {
                            var newCount = Convert.ToInt32(((TextBox) _sender).Text);

                            if (newCount > Miscelaneous.ParticleCount)
                            {
                                Strategy.AddParticles(newCount - Miscelaneous.ParticleCount);
                                Miscelaneous.ParticleCount = newCount;
                            }
                            else
                            {
                                MessageBox.Show("Deleting of particles not implemented, sorry");
                            }
                        }
                        catch
                        {
                        }
                    }
                };

            textBoxSpeed.KeyUp += (_sender, _args) =>
                {
                    if (_args.KeyData == Keys.Enter)
                    {
                        try
                        {
                            var newSpd = Convert.ToDouble(((TextBox) _sender).Text);

                            if (newSpd > MaxSpeedAchieved)
                            {
                                trackBarSpeed.Maximum = (int) (1000*newSpd);
                                MaxSpeedAchieved = newSpd;
                            }
                            trackBarSpeed.Value = (int) (1000*newSpd);
                            Miscelaneous.ParticleSpeed = newSpd;
                        }
                        catch
                        {
                        }
                    }
                };

            trackBarSpeed.ValueChanged += (_sender, _args) =>
                {
                    Miscelaneous.ParticleSpeed = (((TrackBar)_sender).Value / 1000.0);
                    textBoxSpeed.Text = Miscelaneous.ParticleSpeed.ToString();
                };

            textBoxInterractionR.KeyUp += (_sender, _args) =>
                {
                    if (_args.KeyData == Keys.Enter)
                    {
                        try
                        {
                            var newIntR = Convert.ToDouble(((TextBox)_sender).Text);

                            if (newIntR > Miscelaneous.InterractionR)
                            {
                                trackBarInterractionR.Maximum = (int)(1000 * newIntR);
                            }
                            trackBarInterractionR.Value = (int)(1000 * newIntR);
                            Miscelaneous.InterractionR = newIntR;
                        }
                        catch
                        {
                        }
                    }
                };

            trackBarInterractionR.ValueChanged += (_sender, _args) =>
                {
                    Miscelaneous.InterractionR = (((TrackBar)_sender).Value / 1000.0);
                    textBoxInterractionR.Text = Miscelaneous.InterractionR.ToString();
                };

        }

        private void button2_Click(object sender, EventArgs e)
        {
            Strategy.AddParticlesInteresting(1);
        }

    }
}

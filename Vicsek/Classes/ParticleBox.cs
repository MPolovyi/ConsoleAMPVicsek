using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class ParticleBox : IParticleBox
    {
        public IEnumerable<IParticle> Particles { get; private set; }
        private IDrawer Drawer { get; set; }
        private IBorder Border { get; set; }


        public ParticleBox(IEnumerable<IParticle> _particles)
        {
            Particles = _particles;
        }
        
        public ParticleBox(IParticleFactory2D _factory, int _count)
        {
            Particles = _factory.PopulateRandomDistributed(_count);
        }

        public ParticleBox(IParticleFactory2D _factory, int _count, IDrawer _draw)
        {
            Particles = _factory.PopulateRandomDistributed(_count);
            Drawer = _draw;
        }

        public ParticleBox(IParticleFactory2D _factory, int _count, IDrawer _draw, IBorder _border)
        {
            Particles = _factory.PopulateRandomDistributed(_count);
            Drawer = _draw;
            Border = _border;
        }

        public async void NextStep()
        {
            Parallel.ForEach(Particles, _particle => _particle.NextStep(Particles, Border));
            Parallel.ForEach(Particles, _particle => _particle.Move());
            
        }

        public void Draw()
        {
            Drawer.Draw(this);
        }

        public void Draw(PictureBox _bmp)
        {
            Graphics graph = _bmp.CreateGraphics();
            graph.Clear(Color.White);

            Parallel.ForEach(Particles, _particle => _particle.Draw());
        }
    }
}

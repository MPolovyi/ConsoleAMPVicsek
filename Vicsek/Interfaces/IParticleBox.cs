using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Vicsek.Interfaces
{
    public interface IParticleBox
    {
        IEnumerable<IParticle> Particles { get; }
        void NextStep();

        void Draw();
        void Draw(PictureBox _bmp);

    }
}

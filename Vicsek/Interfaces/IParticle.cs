﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Vicsek.Classes;

namespace Vicsek.Interfaces
{
    public interface IParticle
    {
        PairDouble CoordinatesInDouble { get; }
        Point CoordinatesInPoint { get; }
        Point SpeedInPoint { get; }
        PairDouble SpeedInDouble { get; }

        IEnumerable<IParticle> GetNearests(IEnumerable<IParticle> _particles);
        double CalkDistance(IParticle _particle);

        void NextStep(IEnumerable<IParticle> _particles, IBorder _border);
        void Interract();
        void AddNoize(double _noize);
        void Draw(PictureBox _bmp);
        void Draw();

        void RevertSpeed();

        void UpdSpeed(PairDouble _newSpd);
        void UpdCoordinates(PairDouble _newPos);

        void Move();
    }
}

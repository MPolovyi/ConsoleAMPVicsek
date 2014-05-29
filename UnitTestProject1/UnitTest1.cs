using System;
using System.Collections.Generic;
using Microsoft.QualityTools.Testing.Fakes;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Vicsek;
using Vicsek.Classes;
using Vicsek.Classes.Fakes;
using Vicsek.Interfaces;
using Vicsek.Interfaces.Fakes;


namespace UnitTestProject1
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            PairDouble a1 = new PairDouble(0, 0);
            PairDouble b1 = new PairDouble(5, 2);
            PairDouble a2 = new PairDouble(2, 0);
            PairDouble b2 = new PairDouble(0, 5);

            bool areIntersectExpected = true;

            bool areIntersect = Miscelaneous.Intersect(a1, b1, a2, b2);

            Assert.AreEqual(areIntersectExpected, areIntersect);
        }

        [TestMethod]
        public void TestIntersectionAngle()
        {
            var A = new PairDouble(0, 1);
            var B = new PairDouble(2, 0);
            var C = new PairDouble(1, 0);
            var D = new PairDouble(1, 2);

            var ang = Miscelaneous.GetDegreeBetveen(A, B, C, D);

            Assert.AreEqual(ang, 45);
        }

    }
}


using System;
using System.Collections.Generic;
using System.IO;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace DataViewer
{
    class DataConverter
    {
        public List<Tuple<double, double>> GetSpeedOfTime(StreamReader fs)
        {
            List<string> strs = new List<string>();
            while (!fs.EndOfStream)
            {
                Regex dataRegex = new Regex(@"y = \d");
                var str = fs.ReadLine();
                if (dataRegex.IsMatch(str))
                {
                    strs.Add(str);    
                }
            }
            var velocAndNoise = new List<Tuple<double, double>>(strs.Count);
            for (int i = 0; i < strs.Count; i++)
            {
                Regex notNumbers = new Regex(@"[A-Za-z]");
                Regex dotRegex = new Regex(@"\.");
                string veloc = dotRegex.Replace(notNumbers.Replace(strs[i].Split('=')[1],""),",");
                string noise = dotRegex.Replace(notNumbers.Replace(strs[i].Split('=')[2],""),",");

                velocAndNoise.Add(new Tuple<double, double>(Convert.ToDouble(veloc), Convert.ToDouble(noise)));
            }

            return velocAndNoise;
        }


        public List<double> GetSpeedOfTime(string data)
        {
            throw new NotImplementedException();
        }
    }
}

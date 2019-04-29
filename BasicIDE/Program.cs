using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BasicIDE.Helper;
using BasicWrapper;

namespace BasicIDE
{
    class Program
    {
        [STAThread]
        public static void Main(string[] args)
        {
            ConsoleTools.StartupHide();
            ConsoleTools.BackgroundRestore();

            var app = new App();
            app.InitializeComponent();
            app.Run();
        }
    }
}

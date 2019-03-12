using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BasicIDE.Model
{
    public class Editor : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private string text_;

        public string Text
        {
            get { return text_; }
            set { text_ = value; PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Text")); }
        }
    }
}

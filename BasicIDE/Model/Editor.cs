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

        private int tabSize_;
        public int TabSize
        {
            get { return tabSize_; }
            set { tabSize_ = value; PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("TabSize")); }
        }

        private string text_;

        public string Text
        {
            get { return text_; }
            set { text_ = value; PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Text")); }
        }

        private int caretIndex_;

        public int CaretIndex
        {
            get { return caretIndex_; }
            set { caretIndex_ = value; PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("CaretIndex")); }
        }
    }
}

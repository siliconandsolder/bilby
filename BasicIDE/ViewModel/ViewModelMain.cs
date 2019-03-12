using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml.Serialization;
using BasicIDE.Helper;
using BasicIDE.Model;
using Microsoft.Win32;

namespace BasicIDE.ViewModel
{
    public class ViewModelMain : DependencyObject
    {
        private Editor editor;
        public RelayCommand SaveDataCommand { get; set; }
        public RelayCommand LoadDataCommand { get; set; }
        public RelayCommand ExitCommand { get; set; }
        public RelayCommand ExecuteCommand { get; set; }
        public RelayCommand HelpCommand { get; set; }

        public Editor ViewEditor
        {
            get { return editor; }
            set { editor = value; }
        }

        public ViewModelMain()
        {
            editor = new Editor();
            SaveDataCommand = new RelayCommand(SaveData);
            LoadDataCommand = new RelayCommand(LoadData);
            ExitCommand = new RelayCommand(Exit);
            ExecuteCommand = new RelayCommand(Execute);
            HelpCommand = new RelayCommand(Help);
        }

        private void Help(object obj)
        {
            throw new NotImplementedException();
        }

        private void Execute(object obj)
        {
            throw new NotImplementedException();
        }

        private void Exit(object obj)
        {
            Environment.Exit(0);
        }

        private void LoadData(object obj)
        {
            OpenFileDialog open = new OpenFileDialog();
            open.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;
            open.Filter = "Bilby File (*.bil)|*.bil";
            if (open.ShowDialog() == true)
            {
                ViewEditor.Text = File.ReadAllText(open.FileName);
                //XmlSerializer ser = new XmlSerializer(typeof(ObservableCollection<Tile>));
                //StreamReader reader = new StreamReader(open.FileName);
                //ObservableCollection<Tile> TilesTwo = (ObservableCollection<Tile>)ser.Deserialize(reader);

                //for (int i = 0; i < TilesTwo.Count; ++i)
                //    Tiles[i].Colour = new SolidColorBrush((Color)ColorConverter.ConvertFromString(TilesTwo[i].ColourHex));

                //reader.Close();
            }
        }

        private void SaveData(object obj)
        {
            SaveFileDialog save = new SaveFileDialog();
            save.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;
            save.Filter = "Bilby File (*.bil)|*.bil";
            if (save.ShowDialog() == true)
            {
                File.Create(save.FileName).Close();
                File.WriteAllText(save.FileName, ViewEditor.Text);
                //XmlSerializer ser = new XmlSerializer(typeof(ObservableCollection<Tile>));
                //StreamWriter writer = new StreamWriter(save.FileName);
                //ser.Serialize(writer, Tiles);
                //writer.Close();
            }
        }
    }
}

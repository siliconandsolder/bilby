using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml.Serialization;
using BasicIDE.Helper;
using BasicIDE.Model;
using Microsoft.Win32;
using BasicWrapper;
using System.Windows.Controls;

namespace BasicIDE.ViewModel
{
    public class ViewModelMain : DependencyObject
    {
        public RelayCommand SaveDataCommand { get; set; }
        public RelayCommand LoadDataCommand { get; set; }
        public RelayCommand ExitCommand { get; set; }
        public RelayCommand ExecuteCommand { get; set; }
        public RelayCommand HelpCommand { get; set; }
        public RelayCommand TabCommand { get; set; }


        public Editor ViewEditor
        {
            get { return (Editor)GetValue(ViewEditorProperty); }
            set { SetValue(ViewEditorProperty, value); }
        }

        // Using a DependencyProperty as the backing store for ViewEditorPropery.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ViewEditorProperty =
            DependencyProperty.Register("ViewEditorPropery", typeof(Editor), typeof(ViewModelMain), new PropertyMetadata(null));



        public int TabSize
        {
            get { return (int)GetValue(TabSizeProperty); }
            set { SetValue(TabSizeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TabSize.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TabSizeProperty =
            DependencyProperty.Register("TabSize", typeof(int), typeof(ViewModelMain), new PropertyMetadata(null));




        public ViewModelMain()
        {
            ViewEditor = new Editor();
            ViewEditor.TabSize = 4;
            ViewEditor.Text = "";
            SaveDataCommand = new RelayCommand(SaveData);
            LoadDataCommand = new RelayCommand(LoadData);
            ExitCommand = new RelayCommand(Exit);
            ExecuteCommand = new RelayCommand(Execute);
            HelpCommand = new RelayCommand(Help);
            TabCommand = new RelayCommand(Tab);
        }

        private void Tab(object obj)
        {
            TextBox txtBox = obj as TextBox;   
            String tab = new String(' ', ViewEditor.TabSize);
            int newIndex = txtBox.CaretIndex + ViewEditor.TabSize;
            ViewEditor.Text = ViewEditor.Text.Insert(txtBox.CaretIndex, tab);
            txtBox.CaretIndex = newIndex;
        }

        private void Help(object obj)
        {
            throw new NotImplementedException();
        }

        private void Execute(object obj)
        {
            ConsoleTools.Show();
            Wrapper.Execute(ViewEditor.Text);
            Console.WriteLine("\n----------------------------------\nFinished execution. Press any key...");
            Console.ReadKey();
            Console.Clear();
            ConsoleTools.Hide();
        }

        private void Exit(object obj)
        {
            //FreeConsole();
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
            }
        }
    }
}

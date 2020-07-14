using CppReflection.Reflector;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace CppReflection
{
    public class ClassExporter
    {
        private static string Resolve(string from, string to)
        {
            var fromFullPath = Path.GetFullPath(from);
            var toFullPath = Path.GetFullPath(to);

            Uri fromUri = new Uri(fromFullPath);
            Uri toUri = new Uri(toFullPath);

            Uri relativeUri = fromUri.MakeRelativeUri(toUri);
            string relativePath = Uri.UnescapeDataString(relativeUri.ToString());

            if (toUri.Scheme.Equals("file", StringComparison.InvariantCultureIgnoreCase))
            {
                relativePath = relativePath.Replace(Path.AltDirectorySeparatorChar, Path.DirectorySeparatorChar);
            }

            return relativePath;
        }

        public static void OutputCppFile(IEnumerable<TypeDescription> components, string filePath)
        {

            StringBuilder builder = new StringBuilder();
            builder.AppendLine("// This file generated by CppReflection.ClassExpoter.");
            builder.AppendLine("#include \"pch.h\"");
            builder.AppendLine("#include \"DynamicType.hpp\"");
            builder.AppendLine("");
            foreach (var node in components)
            {
                var resolved = Resolve(filePath, node.File).Replace("\\", "/");
                builder.AppendLine($"#include \"{resolved}\"");
            }
            builder.AppendLine("");
            builder.AppendLine("using namespace Egliss::Reflection;");
            builder.AppendLine("using namespace std::string_literals;");
            builder.AppendLine("");
            builder.AppendLine("std::vector<DynamicTypeDescription> DynamicTypeManager::_indexedTypes;");
            builder.AppendLine("std::unordered_map<std::string, int> DynamicTypeManager::_typesindices;");
            builder.AppendLine("");
            builder.AppendLine("");
            builder.AppendLine("void DynamicTypeManager::Initialize()");
            builder.AppendLine("{");
            foreach (var item in components)
            {
                string chainStr = $"std::vector<int>({{{string.Join(" ,", item.ParentChain.Select(m => m.ToString()))}}})";
                string isAbstract = (!item.IsNotAbstract).ToString().ToLower();
                string constructor = item.IsNotAbstract
                    ? "[](){return new " + item.FullName + "(); }"
                    : "[](){return nullptr; }";

                builder.AppendLine($"	_indexedTypes.emplace_back(\"{item.FullName}\", {isAbstract},{chainStr},{constructor});");
                builder.AppendLine($"	_typesindices.emplace(\"{item.FullName}\"s, {item.Id});");
            }
            builder.AppendLine("}");

            string b = builder.ToString();
            using (StreamWriter writer = new StreamWriter(filePath, false))
            {
                writer.Write(b);
            }
        }
        public static void OutputInlineFile(IEnumerable<TypeDescription> components, string filePath)
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("#pragma once");
            builder.AppendLine("// This file generated by CppReflection.ClassExpoter.");

            foreach (var node in components)
            {
                var tmp = node.FullName.Split(new string[] { "::" }, StringSplitOptions.RemoveEmptyEntries);
                var namespaceText = string.Join("::", tmp.Take(tmp.Length - 1));
                builder.AppendLine("namespace " + namespaceText + " { class " + node.Name + "; }");
            }
            builder.AppendLine("");
            builder.AppendLine("namespace Egliss::Reflection");
            builder.AppendLine("{");

            foreach (var node in components)
            {
                string chainCount = node.ParentChain.Count.ToString();
                string chainStr = string.Join(" ,", node.ParentChain.Select(m => m.ToString()));
                string isAbstract = (!node.IsNotAbstract).ToString().ToLower();
                string indent = new string(' ', (node.Indent + 1) * 4);
                /*
                builder.AppendLine(indent + "template<> class StaticTypeDescription<" + node.FullName + ">");
                builder.AppendLine(indent + "{");
                builder.AppendLine(indent + "public:");
                builder.AppendLine(indent + "    static constexpr int Id = " + node.Id.ToString() + ";");
                builder.AppendLine(indent + "    static constexpr std::string_view Name = \"" + node.FullName + "\";");
                builder.AppendLine(indent + "};");
                */
                builder.Append(indent + "template<> class StaticTypeDescription<" + node.FullName + ">");
                builder.Append(" {");
                builder.Append(" public:");
                builder.Append(" static constexpr int Id = " + node.Id.ToString() + ";");
                builder.Append(" static constexpr std::string_view Name = \"" + node.FullName + "\";");
                builder.AppendLine(" };");
            }
            builder.AppendLine("    ");
            builder.AppendLine("}");

            string b = builder.ToString();
            using (StreamWriter writer = new StreamWriter(filePath, false))
            {
                writer.Write(b);
            }
        }
    }
}

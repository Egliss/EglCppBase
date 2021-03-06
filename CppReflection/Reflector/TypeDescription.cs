using Microsoft.VisualStudio.Language.CodeLens.Remoting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CppReflection.Reflector
{
    //write to cpp 
    public class TypeDescription
    {
        public int Id { get; set; }
        public List<int> ParentIds { get; set; } = new List<int>();
        public string Name { get; set; }
        public string FullName { get; set; }
        public bool IsNotAbstract { get; set; }
        public List<int> ParentChain { get; set; } = new List<int>();

        public string File { get; set; }
        public int Indent { get; set; } = 0;

        public static Dictionary<string,TypeDescription> CreateDescription(TypeTreeNode node)
        {
            var descritopns = new Dictionary<string, TypeDescription>();
            CreateDescription(ref descritopns, node, -1);
            return descritopns;
        }

        private static void CreateDescription(ref Dictionary<string,TypeDescription> descriptors, TypeTreeNode node, int parentId, int indent = 0)
        {
            var nodeName = node.Node.FullName.Replace("global::", "");
            TypeDescription target = null;
            // 多重継承 + 2回目以降の出現時
            if(descriptors.TryGetValue(nodeName, out target))
            {
                target.ParentIds.Add(parentId);
            }
            else
            {
                target = new TypeDescription();
                target.Name = node.Node.Name;
                target.FullName = nodeName;
                target.ParentIds.Add(parentId);
                target.Id = descriptors.Count;
                target.Indent = indent;
                target.File = node.Node.VCClass.File;
                target.IsNotAbstract = !node.Node.VCClass.IsAbstract;
                descriptors.Add(target.FullName, target);
            }
            foreach (var item in node.Childs)
            {
                CreateDescription(ref descriptors, item, target.Id, indent + 1);
            }

            MakeParentChain(ref descriptors, target);
        }

        private static void MakeParentChain(ref Dictionary<string, TypeDescription> descriptions, TypeDescription node)
        {
            // TODO 全階層を毎回新規で検索するのでキャッシュすれば良さげ
            var parentChain = new List<int>();
            _MakeParentChain(ref descriptions, node, ref parentChain);

            node.ParentChain = parentChain
                // 多重継承時に同じ親IDを持つ可能性があるので重複排除
                .Distinct()
                // 昇順で二分探索可能にしておく
                .OrderBy(m=>m).ToList();
        }
        private static void _MakeParentChain(ref Dictionary<string, TypeDescription> descriptions, TypeDescription node, ref List<int> ids)
        {
            foreach(var item in node.ParentIds)
            {
                // TODO 許して
                var parent = descriptions.Values.Where(m=>m.Id == item).FirstOrDefault();
                if(parent == null)
                    continue;
                ids.Add(item);
                _MakeParentChain(ref descriptions, parent, ref ids);
            }
        }
    }
}

class NameSpace:
    def __init__(self, name):
        self.name = name
        self.text = "namespace " + name
        self.parent = None
        self.objects = []
    
class Class:
    def __init__(self, name):
        self.name = name
        self.text = "class " + name
        self.bases = []
        self.parent = None
        self.objects = []
    
    def add_base(self, base):
        self.bases.append(base)

class Template:
    def __init__(self, decl):
        self.text = decl
        self.parent = None

class CppFile:
    def __init__(self, name):
        self.name = name
        self.objects = []
        self.current_scope = objects
    
    def open(self, obj):
        obj.parent = current_scope
        self.current_scope.append(obj)
        self.current_scope = obj
    
    def close(self):
        self.current_scope = self.current_scope.parent
    
    def add(self, obj):
        current_scope.objects.append(obj)


class Parser:
    def __init__(self, use_pragma=False):
        self.use_pragma = use_pragma
        self.token_pairs = {
            "(": ")",
            "{": "}",
            "[": "]",
            "<": ">",
            "/*": "*/",
            "//": "\n"
        }
        self.keywords = [ "class", "template", "namespace", "struct" ]
    
    def open(self, f):
        self.input = open(f).read().split()
        self.pos = 0
    
    def match(self, txt):
        if self.input[pos : pos+len(txt)-1] == txt:
            return True
        return False
    
    def match_list(self, lst):
        for txt in lst:
            if self.match(txt):
                return True
        return False
    
    def move(self, txt):
        pos += len(txt)
    
    def move_until(self, txt):
        while not self.match(txt):
            pos += 1
    
    def move_until_closing(self, c):
        start = c[0]
        end = c[1]
        count = 1
        while count:
            if self.match(start):
                count += 1
                move(start)
            else if self.match(end):
                count -= 1
                move(end)
            else:
                pos += 1
    
    def trim_whitespace(self):
        while self.input[pos] in [ " ", "\t", "\n" ]:
            pos += 1
    
    def buffer_start(self):
        self.buffer_pos = pos
    
    def find_keyword(self):
        while not self.match_list(keywords):
            pos += 1
        if self.match("namespace"):
            self.dump_namespace()
        else if self.match("class"):
            self.dump_class()
        else if self.match("struct"):
            self.dump_class()
        else if self.match("template"):
            self.dump_template()
    
    def dump_namespace(self):

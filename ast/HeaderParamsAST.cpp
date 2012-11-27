#include "HeaderParamsAST.hpp"
#include "../idents/Array.hpp"

HeaderParamsAST::HeaderParamsAST(boost::shared_ptr<SymbolTable> st,
                                   pANTLR3_BASE_TREE tree,
                                   boost::shared_ptr<ASTNode> parent,
                                   int lineNo) : ASTNode(st, parent, lineNo) {
    _st = st;
    _tree = tree;
    _lineNo = lineNo;
    check();
}

void HeaderParamsAST::check() {
    for (int i = 0; i + 1 < _tree->getChildCount(_tree); i += 2) {
        string typeString =
          Utils::createStringFromTree(Utils::childByNum(_tree, i));

        if (typeString == "spider") {
            ++i;

            typeString =
              Utils::createStringFromTree(Utils::childByNum(_tree, i));

            string nameString =
              Utils::createStringFromTree(Utils::childByNum(_tree, i + 1));

            boost::shared_ptr<Identifier> type =
              _st->lookupCurrLevelAndEnclosingLevels(typeString);

            boost::shared_ptr<Identifier> name =
              _st->lookupCurrLevelOnly(nameString);

            if (!type) {
                Utils::printSemErr(_lineNo, "Parameter type " + typeString +
                                     " not in scope!");
            } else if (type->getBaseName() != "Type") {
                Utils::printSemErr(_lineNo, "Parameter type " + typeString +
                                     " is not a type.");
            } else if (name) {
                Utils::printSemErr(_lineNo, nameString + " has already been " +
                                     "declared in this scope.");
            } else if (duplicate(i, nameString)) {
                Utils::printSemErr(_lineNo, "Parameter" + nameString +
                                     "already exists.");
            } else {
                boost::shared_ptr<Type> typeCasted =
                  boost::shared_polymorphic_downcast<Type>(type);

                boost::shared_ptr<Type> array(new Array(typeCasted));
                boost::shared_ptr<Param> p(new Param(array, nameString));
                _params.push_back(p);
            }
        } else {
            string nameString =
              Utils::createStringFromTree(Utils::childByNum(_tree, i + 1));

            boost::shared_ptr<Identifier> type =
              _st->lookupCurrLevelAndEnclosingLevels(typeString);

            boost::shared_ptr<Identifier> name =
              _st->lookupCurrLevelOnly(nameString);

            if (!type) {
                Utils::printSemErr(_lineNo, "Parameter type " + typeString +
                                     " not in scope!");
            } else if (type->getBaseName() != "Type") {
                Utils::printSemErr(_lineNo, "Parameter type " + typeString +
                                     " is not a type.");
            } else if (name) {
                Utils::printSemErr(_lineNo, nameString + " has already been " +
                                     "declared in this scope.");
            } else if (duplicate(i, nameString)) {
                Utils::printSemErr(_lineNo, "Parameter" + nameString +
                                     "already exists.");
            } else {
                boost::shared_ptr<Type> typeCasted =
                  boost::shared_polymorphic_downcast<Type>(type);

                boost::shared_ptr<Param> p(new Param(typeCasted, nameString));
                _params.push_back(p);
            }
        }
    }
}

bool HeaderParamsAST::duplicate(int upto, string name) {
    for (int i = 0; i + 1 < upto; i += 2) {
        string nameString =
          Utils::createStringFromTree(Utils::childByNum(_tree, i + 1));

        if (nameString == "spider") {
            nameString =
              Utils::createStringFromTree(Utils::childByNum(_tree, ++i + 1));
        }

        if (nameString == name) {
            return true;
        }
    }
    return false;
}

vector< boost::shared_ptr<Param> > HeaderParamsAST::getParams() {
    return _params;
}

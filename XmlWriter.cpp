#include "XmlWriter.h"
#include "XmlUtils.h"
#include "Indexes/Index.h"
#include "Objects/NamedUser.h"
#include "DumpObjects/DumpPage.h"
#include "DumpObjects/DumpRevision.h"
#include "XML/xmloutput.h"
#include "XML/xmlfile.h"

std::string caseToString(const Case value)
{
    switch (value)
    {
    case Case::CaseSensitive:
        return "case-sensitive";
    case Case::FirstLetter:
        return "first-letter";
    }

    throw DumpException();
}

void XmlWriter::WriteDump(std::shared_ptr<WritableDump> dump, std::string fileName)
{
    XML::FileOutputStream stream(fileName.c_str());

    XML::Output output(stream);

    auto siteInfo = dump->siteInfo->siteInfo;

    output.BeginElementAttrs("mediawiki");
    output.WriteAttr("xmlns", "http://www.mediawiki.org/xml/export-0.8/");
    output.WriteAttr("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    output.WriteAttr("xsi:schemaLocation", "http://www.mediawiki.org/xml/export-0.8/ http://www.mediawiki.org/xml/export-0.8.xsd");
    output.WriteAttr("version", "0.8");
    output.WriteAttr("xml:lang", siteInfo.Lang);
    output.EndAttrs();

    output.BeginElement("siteinfo");

    output.WriteElement("sitename", siteInfo.SiteName);
    output.WriteElement("base", siteInfo.Base);
    output.WriteElement("generator", siteInfo.Generator);
    output.WriteElement("case", caseToString(siteInfo.SiteCase));

    output.BeginElement("namespaces");

    for (auto ns : siteInfo.Namespaces)
    {
        output.BeginElementAttrs("namespace");
        output.WriteAttr("key", ns.first);
        output.WriteAttr("case", caseToString(ns.second.first));
        if (ns.second.second.empty())
            output.EndElementAttrs();
        else
        {
            output.EndAttrs(XML::Output::terse);
            output << ns.second.second;
            output.EndElement(XML::Output::terse);
        }
    }
    output.EndElement();

    output.EndElement();

    int i = 0;
    for (auto pageInfo : *dump->pageIdIndex)
    {
        auto page = DumpPage(dump, pageInfo.second).page;

        output.BeginElement("page");

        output.WriteElement("title", escapeElementText(page.Title));
        output.WriteElement("ns", page.Namespace);
        output.WriteElement("id", page.PageId);

        if (page.RedirectTarget != string())
        {
            output.BeginElementAttrs("redirect");
            output.WriteAttr("title", page.RedirectTarget);
            output.EndElementAttrs();
        }

        int j = 0;
        for (auto revisionId : page.RevisionIds)
        {
            auto revision = DumpRevision(dump, revisionId, false).revision;

            output.BeginElement("revision");

            output.WriteElement("id", revision.RevisionId);
            if (revision.ParentId != 0)
                output.WriteElement("parentid", revision.ParentId);
            output.WriteElement("timestamp", revision.DateTime.ToString());

            output.BeginElementAttrs("contributor");

            if (HasFlag(revision.Flags, RevisionFlags::ContributorDeleted))
            {
                output.WriteAttr("deleted", "deleted");
                output.EndElementAttrs();
            }
            else
            {
                output.EndAttrs();
            
                auto namedUser = std::dynamic_pointer_cast<NamedUser>(revision.Contributor);
                if (namedUser != nullptr)
                {
                    output.WriteElement("username", escapeElementText(revision.Contributor->UserName));
                    output.WriteElement("id", revision.Contributor->UserId);
                }
                else
                {
                    output.WriteElement("ip", revision.Contributor->UserName);
                }

                output.EndElement();
            }

            if (HasFlag(revision.Flags, RevisionFlags::Minor))
                output.WriteElement("minor");

            if (HasFlag(revision.Flags, RevisionFlags::CommentDeleted))
            {
                output.BeginElementAttrs("comment");
                output.WriteAttr("deleted", "deleted");
                output.EndElementAttrs();
            }
            else
            {
                if (revision.Comment.length() > 0)
                    output.WriteElement("comment", escapeElementText(revision.Comment));
            }

            output.BeginElementAttrs("text");

            if (HasFlag(revision.Flags, RevisionFlags::TextDeleted))
                output.WriteAttr("deleted", "deleted");
            else
                output.WriteAttr("xml:space", "preserve");

            if (revision.Text != string())
            {
                output.EndAttrs(XML::Output::terse);
                output << escapeElementText(revision.Text);
                output.EndElement(XML::Output::terse);
            }
            else
                output.EndElementAttrs();

            output.WriteElement("sha1", revision.Sha1);
            output.WriteElement("model", revision.Model);
            output.WriteElement("format", revision.Format);

            output.EndElement();
        }

        output.EndElement();
    }

    output.EndElement();
}
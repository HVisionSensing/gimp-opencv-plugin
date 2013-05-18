/* Gimp OpenCV Plugin
 *
 * Copyright (c) 2013 see AUTHORS file.
 *
 * Gimp OpenCV Plugin is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * Gimp OpenCV Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Gimp OpenCV Plugin. If not, see <http://www.gnu.org/licenses/>.
 */
#include "dilate.hpp"

#include "utility/bundle_widgets.hpp"
#include "utility/interface.hpp"
#include "utility/meta.hpp"
#include "widget/pair_widget.hpp"
#include "widget/mat_widget.hpp"

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>

namespace
{
    typedef boost::tuple<cv::Mat, cv::Point, int, int, cv::Scalar> Arguments;

    boost::optional<Arguments> presentDialog()
    {
        gimp_ui_init("opencv", FALSE);

        GimpDialog* dialog =
            GIMP_DIALOG(gimp_dialog_new("dialog",
                                        "dialog_role",
                                        NULL,
                                        GTK_DIALOG_MODAL,
                                        gimp_standard_help_func,
                                        "opencv_gimp",

                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                        NULL));

        std::vector<boost::tuple<std::string, GtkWidget*> > argumentPairs;

        PairWidget<cv::Size_<int> > ksizeWidget(0, 0);
        argumentPairs.push_back(makeArgumentPair("ksize:", ksizeWidget));

        MatWidget<uint8_t> kernelWidget;
        argumentPairs.push_back(makeArgumentPair("kernel:", kernelWidget));

        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(bundleWidgets(argumentPairs)));
        gtk_widget_show_all(GTK_WIDGET(dialog));

        boost::optional<Arguments> arguments;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            // arguments = Arguments(ksizeWidget, anchorWidget, borderTypeWidget);
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return arguments;
    }

} // namespace

void imgproc::dilate::install()
{
    static GimpParamDef args[] =
        {
            {GIMP_PDB_INT32,    (gchar*)"run-mode", (gchar*)"Run mode"},
            {GIMP_PDB_IMAGE,    (gchar*)"image",    (gchar*)"Input image"},
            {GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable"}
        };

    gimp_install_procedure("dilate",
                           "Dilates an image by using a specific structuring element",
                           "Dilates an image by using a specific structuring element",
                           "Gimp OpenCV Plugin Team",
                           "Gimp OpenCV Plugin Team",
                           "2013",
                           "_dilate",
                           "RGB*, GRAY*",
                           GIMP_PLUGIN,
                           G_N_ELEMENTS(args), 0,
                           args, NULL);
}

void imgproc::dilate::registerName(std::map<std::string, void(*)(GimpRunMode, gint32, gint32)>& runFunctions)
{
    runFunctions["dilate"] = imgproc::dilate::run;
}

void imgproc::dilate::run(GimpRunMode, gint32, gint32 drawableId)
{
    boost::optional<Arguments> arguments = presentDialog();
    if (!arguments)
    {
        return;
    }

    GimpDrawable* const drawable = gimp_drawable_get(drawableId);

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    // cv::dilate(src, dst, UNPACK_TUPLE(*arguments, 0, 1));
    setMatToDrawable(dst, drawable);

    gimp_displays_flush();
    gimp_drawable_detach(drawable);
}